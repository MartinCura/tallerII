DROP TABLE IF EXISTS categories, job_positions, skills cascade;

CREATE TABLE IF NOT EXISTS categories (
  name VARCHAR UNIQUE PRIMARY KEY,
  description VARCHAR
);

CREATE TABLE IF NOT EXISTS job_positions (
  name VARCHAR,
  description VARCHAR,
  category VARCHAR REFERENCES categories (name) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS skills (
  name VARCHAR,
  description VARCHAR,
  category VARCHAR REFERENCES categories (name) ON DELETE CASCADE ON UPDATE CASCADE
);