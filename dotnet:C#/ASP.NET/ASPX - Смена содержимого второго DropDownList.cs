// Есть два списка: при выборе значения в первом, изменяется набор значений во втором

// --- somepage.aspx

// первый лист
<asp:DropDownList runat="server" ID="filter_type"
    onchange="filter_type_changed(this)">
    <asp:ListItem Value=""></asp:ListItem>
    <asp:ListItem Value="1">физическое лицо</asp:ListItem>
    <asp:ListItem Value="2">юридическое лицо</asp:ListItem>
</asp:DropDownList>

// второй лист
<asp:DropDownList runat="server" ID="filter_branch" class="form-control"
    AppendDataBoundItems ="true" ClientIDMode="Static">
    <asp:ListItem Value=""></asp:ListItem>
</asp:DropDownList>

<script type="text/javascript">

    // смена содержимого выпадающего списка
    function rebindDropDownList(dropDownID, listOptions, addEmpty) {
        // очистить содержимое
        $(dropDownID).empty();

        // если надо, добавить первым элементом пустую строку
        if (addEmpty != false) { $(dropDownID).prepend($('<option value=""></option>')); }

        for (var i in listOptions) {
            $(dropDownID).append('<option value="' + i + '">' + listOptions[i] + '</option>');
        }
    };

    // смена списка филиалов при выборе типа лица в фильтре
    function filter_type_changed(obj) {
        jQuery.ajax({
            url: 'somepage.aspx/getBranchList',
            type: "POST",
            data: "{'isUL':'" + $(obj).val() + "'}",
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            success: function (data) {
                rebindDropDownList($("#filter_branch"), data.d, true);
            },
            error: function () { alert("Не удалось получить список филиалов."); }
        });
    }
    
</script>

// --- somepage.cs

/// <summary>
/// Получение списка филиалов в зависимости от типа лица
/// </summary>
/// <param name="isUL">тип лица, ФЛ(0) или ЮЛ(1)</param>
/// <returns>словарь [код филиала]:[название филиала]</returns>
[WebMethod()]
public static object getBranchList(string isUL)
{
    // если выбран первый пункт списка, пустая строка
    if (string.IsNullOrEmpty(isUL)) { return null; }

    Dictionary<string, string> branchList = new Dictionary<string, string>();
    
    if (isUL == "2") // список филиалов для ЮЛ
    {
        using (datasets.somedatasetOne cl = new datasets.somedatasetOne())
        {
            var branches = from c in cl.Branches
                           where c.IsActive == true
                           orderby c.Branch ascending
                           select new { c.IdBranch, c.Branch };
            foreach (var br in branches)
            {
                branchList.Add(br.IdBranch.ToString(), br.Branch);
            }
        }
    }
    else // список филиалов для ФЛ
    {
        using (datasets.somedatasetTwo pu = new datasets.somedatasetTwo())
        {
            var branches = from p in pu.branchFL_zapros
                           orderby p.name ascending
                           select new { p.code, p.name };
            foreach (var br in branches)
            {
                branchList.Add(br.code, br.name);
            }
        }
    }

    return branchList;
}