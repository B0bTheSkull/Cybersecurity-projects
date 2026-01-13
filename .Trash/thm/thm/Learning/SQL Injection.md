---
title: SQL Injection — TryHackMe
tags:
slug: SQL Injection
summary: SQL (Structured Query Language) Injection, mostly referred to as SQLi, is an attack on a web application database server that causes malicious queries to be executed. When a web application communicates with a database using input from a user that hasn't been properly validated, there runs the potential of an attacker being able to steal, delete or alter private and customer data and also attack the web application authentication methods to private or customer areas. This is why SQLi is one of the oldest web application vulnerabilities, and it can also be the most damaging.
---
11-24-2025
17:40
## Introduction

SQL (Structured Query Language) Injection, mostly referred to as SQLi, is an attack on a web application database server that causes malicious queries to be executed. When a web application communicates with a database using input from a user that hasn't been properly validated, there runs the potential of an attacker being able to steal, delete or alter private and customer data and also attack the web application authentication methods to private or customer areas. This is why SQLi is one of the oldest web application vulnerabilities, and it can also be the most damaging.
## Content

### What is a database?

Simply put a way of electronically storing collections of data in an organised manner.
These are all controlled by a `DBMS` - Database Management System.
- Relational: stores the data in tables with predefined schema and uses things like SQL
- Non-relational: key-value, document, or graph for flexible storage of structured, semi-structured, and unstructured data.

SQL (Structured Query Language) is a feature-rich language used for querying databases. These SQL queries are better referred to as statements.

### Blind SQLi - Authentication Bypass
Authentication Bypass 
- One of the most straightforward Blind SQL Injection techniques is when bypassing authentication methods such as login forms. We're not really interested in retrieving data from the database; We just want to get past the login. 
	In more simple terms the web application is asking the database: "Do you have a user with the username bob and the password bob123? If yes, let them enter, else say no no etc."

### Blind SQLi - Boolean Based
Boolean-based SQL Injection refers to the response we receive from our injection attempts, which could be a true/false, yes/no, on/off, 1/0 or any response that can have two outcomes like other boolean responses.

The example given in the task is the following URL `https://website.thm/checkuser?username=admin` which returns the value `{"take":true}`. We can enumerate the database via SQLi and take the responses as True vs False from something as simple as this check username function.

`admin123' UNION SELECT 1,2,3 FROM information_schema.COLUMNS WHERE TABLE_SCHEMA='sqli_three' and TABLE_NAME='users' and COLUMN_NAME like 'a%';`

We can use these wild cards to cycle through characters looking for databases, tables, etc.

For example this is us cycling through utilizing wild cards to find the value of their password:
`admin123' UNION SELECT 1,2,3 from users where username='admin' and password like 'a%`

Obviously this is something I assume we would want to automate to the best of our ability.

### Blind SQLi - Time Based

A time-based blind SQl injection is very similar to the above boolean-based one in that the same requests are being sent, but there is no visual indicator of your queries being wrong or right this time. Instead, the indicator of a correct query is based on the time the query takes to complete. Utilizing

Example this will return a 5 second wait that you'll be able to use like a boolean indicator to enumerate the database and passwords etc.

`admin123' UNION SELECT SLEEP(5);--`

### Out-of-Band SQLi
Out-of-Band SQL Injection isn't as common as it either depends on specific features being enabled on the database server or the web application's business logic, which makes some kind of external network call based on the results from a SQL query. 

Attack channel could be a web request and the data gathering channel could be monitoring HTTP/DNS requests made to a service you control.

### Remediation
As impactful as SQL Injection vulnerabilities are, developers do have a way to protect their web applications from them by following the below advice:

- Prepared Statements (With Parameterized Queries)
	- Write the SQL query first and then any user inputs are added as parameters afterwards. Makes your code look much cleaner and easier to read on top of protecting your database.
- Input Validation
	- Employing an allow list can restrict input to only certain strings, or a string replacement method in the programming language can filter the characters you wish to allow or disallow.
- Escaping User Input
	- Allowing user input containing characters such as ' " $ \ can cause SQL Queries to break or, even worse, as we've learnt, open them up for injection attacks. Escaping user input is the method of prepending a backslash `(\)` to these characters, which then causes them to be parsed just as a regular string and not a special character.

![[Pasted image 20251130192545.png]]


## Skills Learned