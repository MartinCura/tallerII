DROP TABLE IF EXISTS categories cascade;
DROP TABLE IF EXISTS job_positions cascade;

CREATE TABLE categories (
  ID SERIAL PRIMARY KEY,
  name VARCHAR UNIQUE,
  description VARCHAR
);

CREATE TABLE job_positions (
  ID SERIAL PRIMARY KEY,
  name VARCHAR UNIQUE,
  description VARCHAR,
  category VARCHAR REFERENCES categories (name)
);