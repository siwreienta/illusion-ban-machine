CREATE TABLE IF NOT EXISTS users (
    user_id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS tasks (
    task_id SERIAL PRIMARY KEY,
    contest_number INTEGER NOT NULL,
    task_number INTEGER NOT NULL,
    UNIQUE(contest_number, task_number)
);

CREATE TABLE IF NOT EXISTS solutions (
    solution_id SERIAL PRIMARY KEY,
    task_id INTEGER REFERENCES tasks(task_id),
    user_id INTEGER REFERENCES users(user_id),
    code BYTEA NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS graphs (
    graph_id SERIAL PRIMARY KEY,
    solution_id INTEGER REFERENCES solutions(solution_id),
    graph BYTEA NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);