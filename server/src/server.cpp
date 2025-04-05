#include "server.hpp"
#include <fmt/format.h>
#include <fstream>
#include <functional>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/handlers/http_handler_static.hpp>
#include <userver/server/http/http_response_body_stream.hpp>
#include "../../algorithm/interpreter.hpp"
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
        response.SetContentType("text/html");

        return R"(<html>
            <head>
                <title>Illusion | Plagiarism Detection</title>
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
                <h1>Illusion | Plagiarism Detection System</h1>
                <a href="/check-status">Compare Graphs</a>
                <a href="/load-code">Upload Code</a>
            </body>
        </html>)";
    }
};

class CheckStatusController final
    : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-check-status";

    using HttpHandlerBase::HttpHandlerBase;

    std::string
    HandleRequestThrow(const userver::server::http::HttpRequest &request, userver::server::request::RequestContext &)
        const override {
        auto &response = request.GetHttpResponse();
        response.SetContentType("text/html");

        if (request.GetMethod() == userver::server::http::HttpMethod::kPost) {
            const auto file1 = request.GetArg("file1");
            const auto file2 = request.GetArg("file2");
            try {
                std::string status = apotheosis::CheckStatus();
                return fmt::format(
                    R"(<html>
                        <body>
                            <h1>{}</h1>
                            <a href="/">Back to Main</a>
                        </body>
                    </html>)",
                    status
                );
            } catch (std::exception &e) {
                return fmt::format(
                    "<html><body><h1>Error: {}</h1><a "
                    "href='/'>Back</a></body></html>",
                    e.what()
                );
            }
        } else {  // Sorry about it
            return R"(<html>
                <head>
                    <title>Compare Graphs | Illusion</title>
                    <style>
                        body { font-family: Arial, sans-serif; margin: 40px; }
                        form { margin: 20px 0; }
                        input { margin: 10px; padding: 8px; width: 300px; }
                        button { padding: 10px 20px; background: #4CAF50; 
                                color: white; border: none; border-radius: 5px; }
                    </style>
                </head>
                <body>
                    <h1>Compare Graphs: do you wanna do it?</h1>
                    <form method="POST">
                        <button type="submit">Compare</button>
                    </form>
                    <a href="/">Back to Main</a>
                </body>
            </html>)";
        }
    }
};

class LoadCodeController final
    : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-load-code";

    using HttpHandlerBase::HttpHandlerBase;

    std::string
    HandleRequestThrow(const userver::server::http::HttpRequest &request, userver::server::request::RequestContext &)
        const override {
        auto &response = request.GetHttpResponse();
        response.SetContentType("text/html");

        if (request.GetMethod() == userver::server::http::HttpMethod::kPost) {
            try {
                const std::string code1 = request.GetArg("code1");
                const std::string code2 = request.GetArg("code2");

                // std::ofstream file1("../joern_work_folder/test_files/0.cpp");
                // std::ofstream file2("../joern_work_folder/test_files/1.cpp");
                // file1 << code1;
                // file2 << code2;
                // file1.close();
                // file2.close();

                graph_maker::joern_graph_maker joern_parser;
                graph_maker::files_stack file_stack(
                    "../joern_work_folder/test_files"
                );
                joern_parser.clear_directory("../joern_work_folder/results");
                joern_parser.make_graph(file_stack);

                return R"(<html>
                        <body>
                            <h1>Codes saved and graphs generated!</h1>
                            <a href="/check-status">Compare Graphs</a><br>
                            <a href="/load-code">Upload New Codes</a><br>
                            <a href="/">Main Page</a>
                        </body>
                    </html>)";
            } catch (const std::exception &e) {
                return fmt::format(
                    R"(<html>
                            <body>
                                <h1>Error: {}</h1>
                                <a href="/load-code">Try Again</a>
                            </body>
                        </html>)",
                    e.what()
                );
            }
        } else {
            return R"(<html>
                    <head>
                        <title>Illusion | Upload Codes</title>
                        <style>
                            body { font-family: Arial, sans-serif; margin: 40px; }
                            .code-area { 
                                display: flex;
                                gap: 20px;
                                margin-bottom: 20px;
                            }
                            textarea { 
                                width: 100%;
                                height: 300px;
                                font-family: monospace;
                                padding: 10px;
                            }
                            button { 
                                padding: 10px 20px;
                                background: #4CAF50;
                                color: white;
                                border: none;
                                border-radius: 5px;
                                cursor: pointer;
                            }
                        </style>
                    </head>
                    <body>
                        <h1>Upload Codes for Comparison</h1>
                        <form method="POST">
                            <div class="code-area">
                                <div>
                                    <h3>First Code</h3>
                                    <textarea name="code1" placeholder="Enter first code..."></textarea>
                                </div>
                                <div>
                                    <h3>Second Code</h3>
                                    <textarea name="code2" placeholder="Enter second code..."></textarea>
                                </div>
                            </div>
                            <button type="submit">Generate Graphs</button>
                        </form>
                        <a href="/">Back to Main</a>
                    </body>
                </html>)";
        }
    }
};

}  // namespace

std::string CheckStatus() {
    try {
        long double result_of_check = apotheosis::check_two_graphs(
            "../joern_work_folder/results/0.cpp/output.dot",
            "../joern_work_folder/results/1.cpp/output.dot"
        );
        return "Graphs identical for " +
               std::to_string(static_cast<int>(result_of_check * 100)) + "%";
    } catch (std::exception &e) {
        return e.what();
    }
}

std::string LoadCodes() {
    try {
        graph_maker::joern_graph_maker joern_parser;
        graph_maker::files_stack file_stack("../joern_work_folder/test_files");
        joern_parser.clear_directory("../joern_work_folder/results");
        joern_parser.make_graph(file_stack);
        return "Code processed successfully";
    } catch (graph_maker::parcerer_errors &e) {
        return e.what();
    }
}

void AppendMainPage(userver::components::ComponentList &component_list) {
    component_list.Append<MainPageController>();
}

void AppendCheckStatus(userver::components::ComponentList &component_list) {
    component_list.Append<CheckStatusController>();
}

void AppendLoadCodes(userver::components::ComponentList &component_list) {
    component_list.Append<LoadCodeController>();
}

}  // namespace apotheosis