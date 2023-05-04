#include <memory>

#include <odb/transaction.hxx>
#include <odb/mysql/database.hxx>

#include "person.hpp"
#include "person-odb.hxx"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/network/Server.hpp"
#include "oatpp/web/server/HttpRequestHandler.hpp"
#include "db/mysql.h"
#include <nlohmann/json.hpp>
#include "models/questions.hpp"
#include "questions_db.hpp"
#include "questions_db-odb.hxx"
#include "student.hpp"
#include "student-odb.hxx"
#include "oatpp/web/server/interceptor/AllowCorsGlobal.hpp"

using json = nlohmann::json;


#define O_UNUSED(x) (void)x;

using namespace std;

using namespace odb::core;

std::unique_ptr <odb::core::database> db = initMysql();

// 自定义请求处理程序
class Handler : public oatpp::web::server::HttpRequestHandler {
public:
    // 处理传入的请求，并返回响应
    std::shared_ptr <OutgoingResponse> handle(const std::shared_ptr <IncomingRequest> &request) override {
        O_UNUSED(request);
        transaction t(db->begin());
        auto q(true);
        auto r = (db->query<Person>(q));
        json personList;
        for (const Person &person: r) {
            json j = person;
            personList.push_back(person);
        }
        t.commit();

        return ResponseFactory::createResponse(Status::CODE_200, personList.dump());
    }
};

// ImportQuestionsHandler 类，继承自 oatpp::web::server::HttpRequestHandler
class ImportQuestionsHandler : public oatpp::web::server::HttpRequestHandler {

private:
// 管理员密码
    string adminPassword = "123456";

public:
    // 处理传入的请求，并返回响应
    std::shared_ptr <OutgoingResponse> handle(const std::shared_ptr <IncomingRequest> &request) override {
        // 读取请求体中的 json 数据，并将其解析为 ImportQuestionRequest 对象
        ImportQuestionRequest questionRequest = json::parse(
                (string) request->readBodyToString()).get<ImportQuestionRequest>();

        // 如果请求中的密码和管理员密码不匹配
        if (questionRequest.password != adminPassword) {
            // 返回 HTTP 401 响应
            return ResponseFactory::createResponse(Status::CODE_401);
        }

        // 开始数据库事务，以保证所有数据的完整性和一致性
        odb::transaction t(db->begin());
        // 遍历所有的题目，并添加到数据库
        for (auto &text: questionRequest.questions) {
            auto q = Question(text);
            db->persist(q);
        }
        // 提交事务
        t.commit();
        // 返回 HTTP 200 响应
        return ResponseFactory::createResponse(Status::CODE_200);
    }
};

class GetRandomQuestionsHandler : public oatpp::web::server::HttpRequestHandler {
public:
    // 处理传入的请求，并返回响应
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
        // 从请求中获取“limit”查询参数的值
        // oatpp::data::share::StringKeyLabel是一个字符串键类型
        string limitText = request->getQueryParameter(oatpp::data::share::StringKeyLabel("limit"));
        // 将限制文本转换为int类型
        int limit = std::stoi(limitText);
        // 开始ODB数据库事务
        odb::transaction t(db->begin());

        // 使用ODB数据库查询随机的问题
        auto r = db->query<Question>("ORDER BY RAND() LIMIT " + to_string(limit));

        // 创建JSON响应对象
        json resp;
        resp["code"] = 200;
        resp["msg"] = "ok";
        resp["data"] = vector<string>();

        // 遍历所有问题的结果并将其添加到JSON响应中
        for(Question& question: r) {
            resp["data"].push_back(question.getText());
        }
        // 提交ODB数据库事务
        t.commit();

        // 创建HTTP响应并设置内容类型头
        auto response = ResponseFactory::createResponse(Status::CODE_200, resp.dump());
        response->putHeader("content-type", "application/json");

        // 返回HTTP响应
        return response;
    }
};


class GetRandomQuestionByStudentIdHandler : public oatpp::web::server::HttpRequestHandler {

public:
// 处理传入的请求，并返回响应
    std::shared_ptr <OutgoingResponse> handle(const std::shared_ptr <IncomingRequest> &request) override {
        O_UNUSED(request); // 不使用未使用变量的警告

        // 从查询参数中获取学生ID并将其转换为unsigned long long类型
        string student_id_text = request->getQueryParameter(oatpp::data::share::StringKeyLabel("student_id"));
        unsigned long long student_id = std::stoull(student_id_text);

        // 启动数据库事务
        odb::transaction t(db->begin());

        // 查询对应ID的学生
        auto studentResult = db->query<Student>(query<Student>::student_id == student_id);
        Student student1(0, student_id, "", 0);
        Student *student = &student1;
        if (!studentResult.empty()) {
            // 如果存在该学生，使用查询结果中的学生覆盖默认的学生对象
            student = &(*studentResult.begin());
            // 如果该学生已经选取了题目，返回重复请求的响应
            if (student->getSelectedQuestionId() != 0) {
                json resp;
                resp["code"] = 403;
                resp["msg"] = "你已经抽取过了，请不要重复操作！";
                resp["data"] = vector<string>();
                auto questionResult = db->query<Question>(query<Question>::id == student->getSelectedQuestionId());
                if (!questionResult.empty()) {
                    Question q = *questionResult.begin();
                    resp["data"].push_back(q.getText());
                }
                // 创建HTTP响应，设置响应头和响应体
                auto r = ResponseFactory::createResponse(Status::CODE_403, resp.dump());
                r->putHeader("content-type", "application/json");
                return r;
            }
        }

        // 随机查询未被选取过的题目，并设置为该学生选取的题目
        auto r = db->query<Question>("WHERE selected = 0 ORDER BY RAND() LIMIT 1");
        json resp;
        resp["code"] = 200;
        resp["msg"] = "ok";
        resp["data"] = vector<string>();

        if (!r.empty()) {
            // 如果存在未被选取过的题目，将其添加到响应体中并更新题目的选取状态
            Question &question = *r.begin();
            student->setSelectedQuestionId(question.getId());
            db->persist(student);
            resp["data"].push_back(question.getText());
            question.setSelected(true);
            db->update(question);
        } else {
            // 如果不存在未被选取过的题目，返回响应404
            resp["code"] = 404;
            resp["msg"] = "您没有抽取到题目哦";
        }

        // 提交事务
        t.commit();
        // 创建HTTP响应，设置响应头和响应体
        auto response = ResponseFactory::createResponse(Status::CODE_200, resp.dump());
        response->putHeader("content-type", "application/json");
        return response;
    }
};

void run() {
    // 为 HTTP 请求创建路由器
    auto router = oatpp::web::server::HttpRouter::createShared();

    // 路由 GET - "/hello" 请求到处理程序
    router->route("GET", "/hello", std::make_shared<Handler>());

    router->route("POST", "/questions", std::make_shared<ImportQuestionsHandler>());

    router->route("GET", "/questions/random", std::make_shared<GetRandomQuestionsHandler>());

    router->route("GET", "/question", std::make_shared<GetRandomQuestionByStudentIdHandler>());

    // 创建 HTTP 连接处理程序
    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

    /* Add CORS-enabling interceptors */
    connectionHandler->addRequestInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowOptionsGlobal>());
    connectionHandler->addResponseInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowCorsGlobal>());

    // 创建 TCP 连接提供者
    auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared(
            {"0.0.0.0", 18080, oatpp::network::Address::IP_4});

    // 创建服务器，它接受提供的 TCP 连接并将其传递给 HTTP 连接处理程序
    oatpp::network::Server server(connectionProvider, connectionHandler);

    // 打印服务器端口
    OATPP_LOGI("MyApp", "Server running on port %s", connectionProvider->getProperty("port").getData());

    // 运行服务器
    server.run();
}

int main(int argc, char *argv[]) {
    // 初始化 oatpp 环境
    oatpp::base::Environment::init();
    // 运行应用
    run();
    // 销毁 oatpp 环境
    oatpp::base::Environment::destroy();
    return 0;
}