using (SqlConnection sqlConn = new SqlConnection(WebConfigurationManager.ConnectionStrings["someConnStr"].ToString()))
{
    DateTime date1 = DateTime.Today;
    DateTime date2 = DateTime.Today.AddDays(-7);

    string sqlQuery = "SELECT * "
        + "FROM someTable "
        + "WHERE CAST(di.DateDecision AS DATE) = @dd "
            + "AND CAST(np.DateOfBirth AS DATE) = @db;";
    SqlCommand cmd = new SqlCommand(sqlQuery, sqlConn);
    cmd.Parameters.AddWithValue("@dd", date1.Date);
    cmd.Parameters.AddWithValue("@db", date2.Date);
    SqlDataReader reader;
    cmd.Connection = sqlConn;
    try
    {
        sqlConn.Open();
        reader = cmd.ExecuteReader();
        if (!reader.HasRows) { throw new Exception("Не найдено ничего."); }
    }
    catch (Exception ex)
    {
        if (ex.Message != "Не найдено ничего.")
        {
            // возможно, запись в журнал ошибок
        }
        
        // показать пользователю на интерфейсе, что записей не найдено или произошла ошибка
    }
}