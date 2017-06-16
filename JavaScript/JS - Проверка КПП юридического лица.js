<div runat="server" id="kpp" class="input-group">
    <span class="input-group-addon">КПП</span>
    <asp:TextBox runat="server" ID="KPPUL" class="form-control" placeholder="1234LD789" />
</div>

<asp:CustomValidator runat="server" ID="KPPULValid" ControlToValidate="KPPUL" ClientValidationFunction="isKPPvalid" Display="Dynamic" ValidationGroup="ulValids">
    <div class="alert alert-danger">
        <font color="red"><b>КПП</b> не соответствует формату</font>
    </div>
</asp:CustomValidator>

...

<script type="text/javascript">

// проверка КПП на соответствие формату
function isKPPvalid(sender, args) {
    if (args.Value.length != 9) {
        alert("КПП должен представлять собой девятизначный код");
        args.IsValid = false;
        return args.IsValid;
    }
    if (!args.Value.match(/\d{4}[\dA-Z][\dA-Z]\d{3}/)) {
        alert("КПП не соответствует формату");
        args.IsValid = false;
        return args.IsValid;
    }
    else {
        args.IsValid = true;
        return args.IsValid;
    }
}

</script>