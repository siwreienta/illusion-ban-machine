#include "server.hpp"
#include <fmt/format.h>
#include <fstream>
#include <functional>
#include <pqxx/pqxx>
#include <string>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/json.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/server/handlers/http_handler_static.hpp>
#include <userver/server/http/http_response_body_stream.hpp>
#include "../../algorithm/apotheosis.hpp"
#include "../../database/database.hpp"
#include "../../joern_parse/input-parcer.hpp"

namespace apotheosis {

namespace {

class MainPageController final
    : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-main-page";

    using HttpHandlerBase::HttpHandlerBase;

    std::string
    HandleRequestThrow(const userver::server::http::HttpRequest &request, userver::server::request::RequestContext &)
        const override {
        auto &response = request.GetHttpResponse();
        response.SetContentType("text/html; charset=utf-8");
        // SORRY
        return R"(<html>
            <head>
                <title>Illusion</title>
                <style>
                    body { font-family: Arial, sans-serif; margin: 40px; }
                    h1 { color: #333; }
                    a { display: inline-block; margin: 10px; padding: 10px 20px; 
                        background: #4CAF50; color: white; text-decoration: none; 
                        border-radius: 5px; }
                    a:hover { background: #45a049; }
                    form { margin: 20px 0; }
                    textarea { width: 100%; height: 200px; }
                </style>
            </head>
            <body>
                <h1>Иллюзия | Мы всех забаним</h1>
                <a href="/load-code">Апофеоз</a>
            </body>
        </html>)";
    }
};

class LoadCodePageController final
    : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-load-code-page";
    using HttpHandlerBase::HttpHandlerBase;

    std::string
    HandleRequestThrow(const userver::server::http::HttpRequest &request, userver::server::request::RequestContext &)
        const override {
        auto &resp = request.GetHttpResponse();
        resp.SetContentType("text/html; charset=utf-8");  // SORRY
        return R"__(<!DOCTYPE html>
<html>
<head><meta charset="utf-8"><title>Upload Code</title></head>
<body>
  <h1>Да начнется суд! (пожалуйста, не стоит много раз тыкать на кнопку "проверить", запаситесь терпением)</h1>
  <form id="code-form">
    <label>Первый контестант: <input name="user1" required></label><br>
    <label>Второй контестант: <input name="user2" required></label><br>
    <label>Контест: <input name="contest" type="number" value="1"></label><br>
    <label>Задача: <input name="task" type="number" value="1"></label><br>
    <p>Код первого участника:</p>
    <textarea name="code1" rows="10" cols="80"></textarea><br>
    <p>Код второго участника:</p>
    <textarea name="code2" rows="10" cols="80"></textarea><br>
    <button type="submit">Проверить сходство</button>
  </form>
  <pre id="result"></pre>
  <script>
    document.getElementById('code-form').addEventListener('submit', async e => {
      e.preventDefault();
      const form = e.target;
      const data = {
        user1: form.user1.value,
        user2: form.user2.value,
        contest: Number(form.contest.value),
        task: Number(form.task.value),
        code1: form.code1.value,
        code2: form.code2.value
      };
      const resp = await fetch('/load-code', {
        method: 'POST',
        headers: {'Content-Type':'application/json'},
        body: JSON.stringify(data)
      });
      const j = await resp.json();
      document.getElementById('result').textContent =
        resp.ok ? j.data
                : `Ошибка ${j.code}: ${j.message}`;
    });
  </script>
</body>
</html>)__";
    }   
};

class LoadCodeController final
    : public userver::server::handlers::HttpHandlerJsonBase {
public:
    static constexpr std::string_view kName = "handler-load-code";

    LoadCodeController(
        const userver::components::ComponentConfig &config,
        const userver::components::ComponentContext &context
    )
        : HttpHandlerJsonBase(config, context) {
    }

    userver::formats::json::Value
    HandleRequestJsonThrow(const userver::server::http::HttpRequest &, const userver::formats::json::Value &json, userver::server::request::RequestContext &)
        const override {
        const auto user_1 = json["user1"].As<std::string>();
        const auto user_2 = json["user2"].As<std::string>();
        const auto contest = json["contest"].As<int>();
        const auto task = json["task"].As<int>();
        const auto code_1 = json["code1"].As<std::string>();
        const auto code_2 = json["code2"].As<std::string>();
        apotheosis::database db_connect;  // TODO: change default constructor
        int solution_id_1 = db_connect.load_code(user_1, task, contest, code_1);
        int solution_id_2 = db_connect.load_code(user_2, task, contest, code_2);
        const std::string buffer_dir = "../joern_work_folder/buffer/";
        const std::string buffer_1 = buffer_dir + "0.cpp";
        const std::string buffer_2 = buffer_dir + "1.cpp";
        std::ofstream buffer_ostream_1(buffer_1, std::ios::binary);
        buffer_ostream_1 << code_1;
        std::ofstream buffer_ostream_2(buffer_2, std::ios::binary);
        buffer_ostream_2 << code_2;
        graph_maker::joern_graph_maker joern_parser;
        graph_maker::files_stack file_stack("../joern_work_folder/buffer");
        joern_parser.clear_directory("../joern_work_folder/results");
        joern_parser.make_graph(file_stack);
#ifdef APOTHEOSIS_DEBUG
        std::cout << "Ура, графы были успешно загружены в систему\n";
#endif
        long double result_of_check = apotheosis::check_two_graphs(
            "../joern_work_folder/results/0.cpp/output.dot",
            "../joern_work_folder/results/1.cpp/output.dot"
        );
#ifdef APOTHEOSIS_DEBUG
        std::cout << "Ура, коды были успешно проверены, их сходство: "
                  << static_cast<int>(result_of_check * 100) << "\n";
#endif
        return userver::formats::json::MakeObject(
            "data",
            "Graphs identical for " +
                std::to_string(static_cast<int>(result_of_check * 100)) + "%"
        );
    }
};

}  // namespace

void AppendMainPage(userver::components::ComponentList &component_list) {
    component_list.Append<MainPageController>();
}

void AppendLoadCodes(userver::components::ComponentList &component_list) {
    component_list.Append<LoadCodeController>();
}

void AppendLoadCodePage(userver::components::ComponentList &component_list) {
    component_list.Append<LoadCodePageController>();
}

}  // namespace apotheosis