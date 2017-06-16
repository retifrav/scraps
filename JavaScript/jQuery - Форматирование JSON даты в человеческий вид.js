// форматирование JSON даты и времени в формат "dd.MM.yyyy hh:mm:ss"
function formatTheDate(JSONdate) {
   var dateFromJSON = new Date(parseInt(JSONdate.substr(6)));
   return dateFromJSON.format("dd.MM.yyyy") + " " // дата
        + dateFromJSON.toLocaleTimeString();      // время в 24-часовом
};
