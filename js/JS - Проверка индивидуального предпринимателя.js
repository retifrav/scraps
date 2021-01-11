<div class="input-group">
    <span class="input-group-addon">ИНН</span>
    <asp:TextBox runat="server" ID="INNIP" class="form-control"
        placeholder="123456789012" />
</div>

 <asp:CustomValidator runat="server" ID="INNIPValid" ControlToValidate="INNIP" ClientValidationFunction="isINNIPvalid" Display="Dynamic" ValidationGroup="ipValids">
    <div class="alert alert-danger">
        <font color="red"><b>ИНН</b> должен быть в числовом формате и состоять из 12 цифр</font>
    </div>
</asp:CustomValidator>

...

<script type="text/javascript">

// проверка ИНН для ИП
function isINNIPvalid(sender, args) {
    // проверка на число
    if (args.Value.match(/\D/)) {
        alert("Введённый ИНН не является числом");
        args.IsValid = false;
        return args.IsValid;
    }
    // проверка на 12 цифр
    if (args.Value.length != 12) {
        alert("ИНН для индивидуального предпринимателя должен состоять из 12 цифр");
        args.IsValid = false;
        return args.IsValid;
    }
    // проверка по контрольным цифрам
    var dgt11 = String(((
        7 * args.Value[0] + 2 * args.Value[1] + 4 * args.Value[2] +
        10 * args.Value[3] + 3 * args.Value[4] + 5 * args.Value[5] +
        9 * args.Value[6] + 4 * args.Value[7] + 6 * args.Value[8] +
        8 * args.Value[9]) % 11) % 10);
    var dgt12 = String(((
        3 * args.Value[0] + 7 * args.Value[1] + 2 * args.Value[2] +
        4 * args.Value[3] + 10 * args.Value[4] + 3 * args.Value[5] +
        5 * args.Value[6] + 9 * args.Value[7] + 4 * args.Value[8] +
        6 * args.Value[9] + 8 * args.Value[10]) % 11) % 10);
    if (args.Value[10] == dgt11 && args.Value[11] == dgt12)
    {
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