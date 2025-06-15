#include "handlers.hpp"

namespace apotheosis {

std::string LoadCodePageController::
    HandleRequestThrow(const userver::server::http::HttpRequest &, userver::server::request::RequestContext &)
        const {
    auto &resp = const_cast<userver::server::http::HttpResponse &>(
        static_cast<const decltype(resp) &>(resp)
    );
    resp.SetContentType("text/html; charset=utf-8");
    return R"__(<!DOCTYPE html>
<html><head><meta charset="utf-8"><title>Upload Code</title></head><body>
<h1>Да начнется суд! ...</h1>
<form id="code-form">...</form>
<pre id="result"></pre>
<script>
document.getElementById('code-form').addEventListener('submit',async e=>{
  e.preventDefault();
  const f=e.target;
  const d={user1:f.user1.value,user2:f.user2.value,
    contest:Number(f.contest.value),task:Number(f.task.value),
    code1:f.code1.value,code2:f.code2.value};
  const r = await fetch('/load-code',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(d)});
  const j = await r.json();
  document.getElementById('result').textContent = r.ok?j.data:`Ошибка ${j.code}:${j.message}`;
});
</script>
</body></html>)__";
}

userver::formats::json::Value LoadCodeController::
    HandleRequestJsonThrow(const userver::server::http::HttpRequest &, const userver::formats::json::Value &json, userver::server::request::RequestContext &)
        const {
    std::string u1 = json["user1"].As<std::string>();
    std::string u2 = json["user2"].As<std::string>();
    int contest = json["contest"].As<int>();
    int task = json["task"].As<int>();
    std::string c1 = json["code1"].As<std::string>();
    std::string c2 = json["code2"].As<std::string>();
    std::string res = ProcessPair(u1, u2, contest, task, c1, c2);
    return userver::formats::json::MakeObject("data", res);
}

std::string LoadCodeController::ProcessPair(
    const std::string &user1,
    const std::string &user2,
    int contest,
    int task,
    const std::string &code1,
    const std::string &code2
) const {
    database db;
    int sid1 = db.load_code(user1, task, contest, code1);
    int sid2 = db.load_code(user2, task, contest, code2);

    const std::string buf = "../joern_work_folder/buffer/";
    const std::string res = "../joern_work_folder/results/";
    std::ofstream(buf + "0.cpp") << code1;
    std::ofstream(buf + "1.cpp") << code2;

    graph_maker::joern_graph_maker parser;
    parser.clear_directory(res);
    parser.make_graph(graph_maker::files_stack(buf));

    long double pct = apotheosis::check_two_graphs(
        res + "0.cpp/output.dot", res + "1.cpp/output.dot"
    );
    int ipct = static_cast<int>(pct * 100);
    return "Codes identical for " + std::to_string(ipct) + "%";
}

userver::formats::json::Value ManyCodesCheck::
    HandleRequestJsonThrow(const userver::server::http::HttpRequest &, const userver::formats::json::Value &, userver::server::request::RequestContext &)
        const {
    return userver::formats::json::MakeObject("data", ProcessBulk());
}

std::string ManyCodesCheck::ProcessBulk() const {
    database db;
    auto entries = db.load_all_submissions();
    const std::string buf = "../joern_work_folder/buffer/";
    const std::string res = "../joern_work_folder/results/";
    for (size_t i = 0; i < entries.size(); ++i) {
        auto &[uid, contest, task, code] = entries[i];
        std::ofstream(buf + std::to_string(i) + ".cpp") << code;
    }

    graph_maker::joern_graph_maker parser;
    parser.clear_directory(res);
    parser.make_graph(graph_maker::files_stack(buf));

    auto rc = apotheosis::check_all_in_dir(res);
    std::string out;
    for (size_t i = 0; i < rc.size(); ++i) {
        for (size_t j = i + 1; j < rc[i].size(); ++j) {
            int p = static_cast<int>(rc[i][j] * 100);
            if (p < 50) {
                continue;
            }
            int u1 = std::get<0>(entries[i]);
            int u2 = std::get<0>(entries[j]);
            out += fmt::format(
                "Есть проблемы: {} (ID {}) и {} (ID {}) — {}%\n",
                db.get_user_name(u1), u1, db.get_user_name(u2), u2, p
            );
        }
    }
    return out.empty() ? "Все хорошо, все молодцы" : out;
}

std::string MainPageController::
    HandleRequestThrow(const userver::server::http::HttpRequest &, userver::server::request::RequestContext &)
        const {
    return R"__(<!DOCTYPE html>
<html><head><meta charset="utf-8"><title>Illusion</title></head><body>
<h1>Иллюзия | Проверка кодов</h1>
<a href="/load-code">Проверка двух</a><br>
<button id="bulk-btn">Проверить всех сразу</button>
<pre id="bulk-result" style="white-space: pre-wrap;"></pre>
<script>
document.getElementById('bulk-btn').onclick=async()=>{
  const r=await fetch('/handler-get-all-codes',{method:'POST'});
  const j=await r.json();
  document.getElementById('bulk-result').textContent=j.data;
};
</script>
</body></html>)__";
}

}  // namespace apotheosis
