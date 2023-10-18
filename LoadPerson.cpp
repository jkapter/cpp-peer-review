
struct DBParams {
    string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
};

DBHandler DBHandlerBuilder(const DBParams& params) {
    DBConnector connector(params.db_allow_exceptions, params.db_log_level);
    if (params.db_name.starts_with("tmp."s)) {
        return connector.ConnectTmp(params.db_name, params.db_connection_timeout);
    } else {
        return connector.Connect(params.db_name, params.db_connection_timeout);
    }
}

optional<vector<Person>> LoadPersons(const DBParams& db_params, const int min_age, const int max_age, const string_view name_filter) {
    
    DBHandler db = move(DBHandlerBuilder(db_params));

    if (!db_params.db_allow_exceptions && !db.IsOK()) {
        return nullopt;
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << min_age << " and "s << max_age << " "s
              << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}