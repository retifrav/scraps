using System.DirectoryServices;
using System.DirectoryServices.ActiveDirectory;

// получаем все домены
DomainCollection domains = Forest.GetCurrentForest().Domains;
// и перебираем их
foreach (var domain in domains)
{
    // строка подключения к очередному
    string connStr = "LDAP://" + domain;
    // входим в домен
    DirectoryEntry de = new DirectoryEntry(connStr);
    // создаём поисковик и присваеваем ему строку фильтра
    DirectorySearcher mySearcher = new DirectorySearcher(de);
    // искать будем по e-mail
    mySearcher.Filter = "(&(objectClass=user)(mail=" + "vasya@somedomain.net" + "))";
    // запускаем поиск (здесь нам нужен только один)
    SearchResultCollection sr = mySearcher.FindAll();
    // если нашёл, то в других доменах искать уже незачем
    if (sr.Count != 0) break;
}

// искать можно любые поля по любым полям. Например, обратная ситуация: найти e-mail, зная ФИО. Чтобы въехать подробнее - изучите весь набор полей, который есть у записи в AD.