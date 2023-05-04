# cpp-random-questions

1、自动随机抽题系统后端
总体要求：
编程语言C++，程序主体部分需包含课程讲过的核心知识点
功能包括：
1、设计简单友好的GUI界面（推荐使用Qt）
2、题目快捷导入和存储
3、输入学号随机抽取一题，重复输入学号无效
4、在不输入学号的情况下，配有按键进行抽题，此时可以从试题库中随机抽取6道题
5、以上为离线版本，可结合互联网进行在线抽题

本项目基于 Ubuntu 系统开发，依赖 MySQL, ODB, OATPP, 运行前先配置MySQL，在 db 目录下

下面是 Mysql 表结构
```sql
create or replace table questions.person
(
    id     bigint unsigned auto_increment
        primary key,
    first  text              not null,
    second text              not null,
    age    smallint unsigned not null
);

create or replace table questions.questions
(
    id              int unsigned auto_increment
        primary key,
    text            text       null,
    answer          text       null,
    can_view_answer tinyint(1) null,
    selected        tinyint(1) null
);

create or replace table questions.users
(
    id                   int unsigned auto_increment
        primary key,
    student_id           bigint       null,
    selected_question_id int unsigned null,
    my_answer            text         null,
    constraint users_pk
        unique (student_id)
);

```
