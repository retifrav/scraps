<!-- добавление строки подлючения в Web.config -->

<connectionStrings>
    <!-- вот эту будем использовать, аутентификация стандартная Windows -->
    <add 
        name="local"
        connectionString="Data Source=localhost;Initial Catalog=tempBase;Integrated Security=SSPI"
        providerName="System.Data.SqlClient"
        />
    <!-- эта просто для примера, аутентификация по логину и паролю -->
    <add 
        name="someExternal"
        connectionString="Data Source=192.168.56.22;Initial Catalog=someOtherBase;User ID=vasya;Password=12321;"
        providerName="System.Data.SqlClient"
        />
</connectionStrings>

<!-- теперь где-то в начале разметке страницы указываем источник данных -->

<asp:SqlDataSource
    ID="branchFLsrc"
    runat="server"
    ConnectionString="<%$ ConnectionStrings:local %>"
    SelectCommand = "SELECT * FROM tempBase.dbo.someTable;"
    />

<!-- и создаём сам выпадающий список -->
    
<asp:DropDownList
    runat="server"
    ID="branchFL"
    Width="200px"
    DataSourceid="branchFLsrc"
    DataTextField="name"      // что отображается в списке
    DataValueField="code"     // что будет отправляться при выборе
    />