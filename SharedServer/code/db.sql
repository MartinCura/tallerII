DROP TABLE IF EXISTS categories, job_positions, skills cascade;

CREATE TABLE categories (
  name VARCHAR UNIQUE PRIMARY KEY,
  description VARCHAR
);

CREATE TABLE job_positions (
  name VARCHAR,
  description VARCHAR,
  category VARCHAR REFERENCES categories (name) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE skills (
  name VARCHAR,
  description VARCHAR,
  category VARCHAR REFERENCES categories (name) ON DELETE CASCADE ON UPDATE CASCADE
);