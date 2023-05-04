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

std::unique_ptr<odb::core::database> db = initMysql();

// 自定义请求处理程序
class Handler : public oatpp::web::server::HttpRequestHandler {
public:
    // 处理传入的请求，并返回响应
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest> &request) override {
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

class ImportQuestionsHandler : public oatpp::web::server::HttpRequestHandler {

private:
    string adminPassword = "123456";

public:
    // 处理传入的请求，并返回响应
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest> &request) override {
        O_UNUSED(request);
        ImportQuestionRequest questionRequest = json::parse(
                (string) request->readBodyToString()).get<ImportQuestionRequest>();

        if (questionRequest.password != adminPassword) {
            return ResponseFactory::createResponse(Status::CODE_401);
        }

        odb::transaction t(db->begin());
        for (auto &text: questionRequest.questions) {
            auto q = Question(text);
            db->persist(q);
        }
        t.commit();
        return ResponseFactory::createResponse(Status::CODE_200);
    }
};

class GetRandomQuestionsHandler : public oatpp::web::server::HttpRequestHandler {

public:
    // 处理传入的请求，并返回响应
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest> &request) override {
        O_UNUSED(request);

        string limitText = request->getQueryParameter(oatpp::data::share::StringKeyLabel("limit"));
        int limit = std::stoi(limitText);
//        auto r (db->query<Question> (query<Question>::);
        odb::transaction t(db->begin());
        auto r = db->query<Question>("ORDER BY RAND() LIMIT " + to_string(limit));
        json resp;
        resp["code"] = 200;
        resp["msg"] = "ok";
        resp["data"] = vector<string>();
        for (Question &question: r) {
            resp["data"].push_back(question.getText());
        }
        t.commit();
        auto response = ResponseFactory::createResponse(Status::CODE_200, resp.dump());
        response->putHeader("content-type", "application/json");
        return response;
    }
};

class GetRandomQuestionByStudentIdHandler : public oatpp::web::server::HttpRequestHandler {

public:
    // 处理传入的请求，并返回响应
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest> &request) override {
        O_UNUSED(request);

        string student_id_text = request->getQueryParameter(oatpp::data::share::StringKeyLabel("student_id"));
        unsigned long long student_id = std::stoull(student_id_text);

        odb::transaction t(db->begin());

        auto studentResult = db->query<Student>(query<Student>::student_id == student_id);
        Student student1(0, student_id, "", 0);
        Student *student = &student1;
        if (!studentResult.empty()) {
            student = &(*studentResult.begin());
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
                auto r = ResponseFactory::createResponse(Status::CODE_403, resp.dump());
                r->putHeader("content-type", "application/json");
                return r;
            }
        }

        auto r = db->query<Question>("WHERE selected = 0 ORDER BY RAND() LIMIT 1");
        json resp;
        resp["code"] = 200;
        resp["msg"] = "ok";
        resp["data"] = vector<string>();

        if (!r.empty()) {
            Question &question = *r.begin();
            student->setSelectedQuestionId(question.getId());
            db->persist(student);
            resp["data"].push_back(question.getText());
            question.setSelected(true);
            db->update(question);
        } else {
            resp["code"] = 404;
            resp["msg"] = "您没有抽取到题目哦";
        }

        t.commit();
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
//    odb::session session;
//    // Create the table if it doesn't exist
//    odb::schema_catalog::create_schema(db);
    // 初始化 oatpp 环境
    oatpp::base::Environment::init();
    // 运行应用
    run();
    // 销毁 oatpp 环境
    oatpp::base::Environment::destroy();
    return 0;
}