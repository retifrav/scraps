Например, хотим такую структуру:

- адрес
- номер
- причина
- директора - 
             |
              - главный директор, Иванов, 02.01.1980, Москва
             |
              - второй директор, Петров, 03.04.1982, Москва
             |
              - ещё один, Николаев, 12.02.1976, Самара
             |
              - и так далее

Для этого:

// сбор директоров
var firstmen = [];
$("#container4firstMen").find('div[class="yafm"]').each(function () {
    firstmen.push({
        chiefPost: $(this).find('input[class*="chiefPost"]').val(),
        fio: $(this).find('input[class*="FIO"]').val(),
        prevfio: $(this).find('input[class*="prevFIO"]').val(),
        dob: $(this).find('input[class*="dob"]').val(),
        dop: $(this).find('input[class*="pob"]').val(),
        address: $(this).find('input[class*="address"]').val(),
        passport_series: $(this).find('input[class*="passport_series"]').val(),
        passport_number: $(this).find('input[class*="passport_number"]').val(),
        passport_date: $(this).find('input[class*="passport_date"]').val(),
        passport_from: $(this).find('input[class*="passport_from"]').val()
    });
});

// сбор из одиночных полей
var JSONinfo = {
    address: $("#address").val(),
    number: $("#number").val(),
    reason: $("#reason").val(),
    firstmen: firstmen
};

// и в запрос
jQuery.ajax({
    url: 'somepage.aspx/saveAdditionalInfo',
    type: "POST",
    data: JSON.stringify({'JSONinfo': JSONinfo}),
    contentType: "application/json; charset=utf-8",
    dataType: "json",
    success: function () { alert("Успешно"); },
    error: function () { alert("Ошибка"); }
});