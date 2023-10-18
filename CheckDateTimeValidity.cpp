
enum class DateTimeError {
    NO_ERROR,
    TOO_SMALL,
    TOO_BIG
};

enum class DateTimePart {
    NO,
    YEAR,
    MONTH,
    DAY,
    HOUR,
    MINUTE,
    SECOND,

};

struct DateTimePartErr {
    DateTimePart part = DateTimePart::NULL;
    DateTimeError err = DateTimeError::NO_ERROR;
};

class DateTimeChecker {
public:
    DateTimeChecker(const DateTime& dt) 
    : dt_(dt)
    {}

    DateTimeError CheckYear() const {
        if(dt_.year < 1 || dt_.year > 9999) {
            return dt_.year < 1 ? DateTimeError::TOO_SMALL : DateTimeError::TOO_BIG;
        } else {
            return DateTimeError::NO_ERROR;
        }
    }

    DateTimeError CheckMonth() const {
        if(dt_.month < 1 || dt_.month > 12) {
            return dt_.month < 1 ? DateTimeError::TOO_SMALL : DateTimeError::TOO_BIG;
        } else {
            return DateTimeError::NO_ERROR;
        }
    }

    DateTimeError CheckDay() const {
        const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
        const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        if(dt_.day < 1 || dt_.day >  month_lengths[dt.month - 1]) {
            return dt_.day < 1 ? DateTimeError::TOO_SMALL : DateTimeError::TOO_BIG;
        } else {
            return DateTimeError::NO_ERROR;
        }
    }

    DateTimeError CheckHour() const {
        if(dt_.hour < 0 || dt_.hour > 23) {
            return dt_.hour < 0 ? DateTimeError::TOO_SMALL : DateTimeError::TOO_BIG;
        } else {
            return DateTimeError::NO_ERROR;
        }
    }

    DateTimeError CheckMinute() const {
        if(dt_.minute < 0 || dt_.minute > 12) {
            return dt_.minute < 0 ? DateTimeError::TOO_SMALL : DateTimeError::TOO_BIG;
        } else {
            return DateTimeError::NO_ERROR;
        }
    }

    DateTimeError CheckSecond() const {
        if(dt_.second < 0 || dt_.second > 59) {
            return dt_.second < 0 ? DateTimeError::TOO_SMALL : DateTimeError::TOO_BIG;
        } else {
            return DateTimeError::NO_ERROR;
        }
    }

    DateTimePartErr CheckAll() {
        DateTimeError err;

        err = CheckYear();
        if(err != DateTimeError::NO_ERROR) {
            return {DateTimePart::YEAR, err};
        }

        err = CheckMonth();
        if(err != DateTimeError::NO_ERROR) {
            return {DateTimePart::MONTH, err};
        }

        err = CheckDay();
        if(err != DateTimeError::NO_ERROR) {
            return {DateTimePart::DAY, err};
        }

        err = CheckHour();
        if(err != DateTimeError::NO_ERROR) {
            return {DateTimePart::HOUR, err};
        }

        err = CheckMinute();
        if(err != DateTimeError::NO_ERROR) {
            return {DateTimePart::MINUTE, err};
        }

        err = CheckSecond();
        if(err != DateTimeError::NO_ERROR) {
            return {DateTimePart::SECOND, err};
        }

        return {DateTimePart::NULL, DateTimeError::NO_ERROR};
    }

    optional<string> GetTextError() const {
        DateTimePartErr err = CheckAll();
        if(err.err == DateTimeError::NO_ERROR) {
            return nullopt;
        };

        string ret_str = part_data_text_[static_cast<size_t>(err.part)];

        if(err.err == DateTimeError::TOO_BIG) {
            ret_str += "big"s;
        } else {
            ret_str += "small"s;
        }

        return ret_str;

    }

private:
    const DateTime& dt_;
    const vector<string> part_data_text_ = {
        ""s,
        "year is too "s,
        "month is too "s,
        "day is too "s,
        "hour is too "s,
        "minute is too "s,
        "second is too "s
    };
};


void CheckDateTimeValidity(const DateTime& dt) {
    DateTimeChecker checker(dt);
    
    DateTimePartErr dt_err_code = checker.CheckAll();

    if(dt_err_code.err == DateTimeError::NO_ERROR) {
        return;
    }

    throw domain_error(checker.GetTextError());
}