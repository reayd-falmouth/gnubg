# Copyright (C) 2004 Joern Thyssen <jth@gnubg.org>
# Copyright (C) 2004-2026 the AUTHORS

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

connection = 0


def PyMySQLConnect(database, user, password, hostname):
    global connection

    try:
        import MySQLdb
    except ImportError:
        # See if pymsql (pure Python replacement) is available. Works on MS
        # Windows
        import pymysql as MySQLdb

    hostport = hostname.strip().split(':')
    try:
        mysql_host = hostport[0]
    except IndexError:
        mysql_host = 'localhost'

    try:
        mysql_portstr = hostport[1]
        try:
            mysql_port = int(mysql_portstr)
        except ValueError:
            return -1
    except IndexError:
        mysql_port = 3306

    try:
        connection = MySQLdb.connect(
            host=mysql_host, port=mysql_port, db=database, user=user, passwd=password)
        return 1
    except Exception:
        # See if mysql is there
        try:
            connection = MySQLdb.connect(
                host=mysql_host, port=mysql_port, user=user, passwd=password)
            # See if database present
            cursor = connection.cursor()
            r = cursor.execute(
                'show databases where `Database` = "' + database + '";')
            if (r != 0):
                return -2  # failed
            cursor.execute('create database ' + database)
            connection = MySQLdb.connect(
                host=mysql_host, port=mysql_port, db=database, user=user, passwd=password)
            return 0
        except Exception:
            return -1  # failed

    return connection


def PyPostgreConnect(database, user, password, hostname):
    global connection
    import psycopg
    from psycopg import sql

    hostport = hostname.strip().split(':')
    try:
        postgres_host = hostport[0]
    except IndexError:
        postgres_host = 'localhost'

    try:
        postgres_portstr = hostport[1]
        try:
            postgres_port = int(postgres_portstr)
        except ValueError:
            return -1
    except IndexError:
        postgres_port = 5432

    try:
        connection = psycopg.connect(
            host=postgres_host, port=postgres_port,
            user=user, password=password, dbname=database, autocommit=True)
        return 1
    except Exception:
        # See if postgres is there
        try:
            # See if database present
            connection = psycopg.connect(
                host=postgres_host, user=user, password=password,
                dbname='postgres', autocommit=True)

            with connection.cursor() as cursor:
                cursor.execute(
                    'SELECT datname FROM pg_database WHERE datname = %s',
                    (database,),
                )
                if cursor.fetchone() is not None:
                    return -2

                cursor.execute(
                    sql.SQL('CREATE DATABASE {}').format(
                        sql.Identifier(database)
                    )
                )

            connection.close()
            connection = psycopg.connect(
                host=postgres_host, dbname=database, user=user,
                password=password, autocommit=True)
            return 0
        except Exception:
            return -1  # failed

    return connection


def PySQLiteConnect(dbfile):
    global connection
    from sqlite3 import dbapi2 as sqlite
    connection = sqlite.connect(dbfile)
    return connection


def PyDisconnect():
    global connection
    try:
        connection.close()
    except Exception:
        pass


def PySelect(str):
    global connection
    cursor = connection.cursor()
    try:
        cursor.execute("SELECT " + str)
    except Exception:
        return 0

    all = list(cursor.fetchall())
    if (len(all) > 0):
        titles = [cursor.description[i][0]
                  for i in range(len(cursor.description))]
        all.insert(0, titles)

    return all


def PyUpdateCommand(stmt):
    global connection
    cursor = connection.cursor()
    cursor.execute(stmt)


def PyUpdateCommandReturn(stmt):
    global connection
    cursor = connection.cursor()
    cursor.execute(stmt)
    return list(cursor.fetchall())


def PyCommit():
    global connection
    connection.commit()
