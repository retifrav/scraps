// проверка даты по формату ДД.ММ.ГГГГ
function isDateCorrect(val4check) {
    // сначала по регулярному выражению
    if (!val4check.match(/^\d\d.\d\d.\d\d\d\d$/)) { return false; }
    // год, месяц, день
    year = parseInt(val4check.substr(6, 4), 10);
    month = parseInt(val4check.substr(3, 2), 10);
    day = parseInt(val4check.substr(0, 2), 10);
    
    // 31 день
    if (day > 31) { return false; }
    // 12 месяцев
    if (month > 12) { return false; }
    
    // 30 дней
    if (month == 4
        || month == 6
        || month == 9
        || month == 11)
    { if (day > 30) return false; }

    // февраль
    if (month == 2) {
        if (new Date(year, 1, 29).getMonth() === 1) { if (day > 29) { return false; } }
        else { if (day > 28) { return false; } }
    }

    // это кошерная дата
    return true;
}