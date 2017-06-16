<div class="input-group">
    <span class="input-group-addon">ИНН</span>
    <asp:TextBox runat="server" ID="INNUL" class="form-control"
        placeholder="1234567890" />
</div>

<asp:CustomValidator runat="server" ID="INNULValid" ControlToValidate="INNUL" ClientValidationFunction="isINNULvalid" Display="Dynamic" ValidationGroup="ulValids">
    <div class="alert alert-danger">
        <font color="red"><b>ИНН</b> должен быть в числовом формате и состоять из 10 цифр</font>
    </div>
</asp:CustomValidator>

...

<script type="text/javascript">

// проверка ИНН для ЮЛ
function isINNULvalid(sender, args) {            
    // проверка на число
    if (args.Value.match(/\D/))
    {
        alert("Введённый ИНН не является числом");
        args.IsValid = false;
        return args.IsValid;
    }
    // проверка на 10 цифр
    if (args.Value.length != 10) {
        alert("ИНН для юридического лица должен состоять из 10 цифр");
        args.IsValid = false;
        return args.IsValid;
    }
    // проверка по контрольным цифрам
    var dgt10 = String(((
        2 * args.Value[0] + 4 * args.Value[1] + 10 * args.Value[2] +
        3 * args.Value[3] + 5 * args.Value[4] + 9 * args.Value[5] +
        4 * args.Value[6] + 6 * args.Value[7] + 8 * args.Value[8]) % 11) % 10);
    if (args.Value[9] == dgt10) {
        args.IsValid = true;
        return args.IsValid;
    }
    else {
        alert("Введённый ИНН не прошёл проверку по контрольным цифрам.");
        args.IsValid = false;
        return args.IsValid;
    }
}

</script>