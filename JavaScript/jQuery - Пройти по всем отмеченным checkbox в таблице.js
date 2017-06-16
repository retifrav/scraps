function assignMulti() {
    $("#tableAll").find('input[type="checkbox"]:checked').each(function () {
        alert(this.attributes["data-id"].value);
    });
}