Например, есть таблица:

<table id="attributes">
    <thead>
        <tr class="active">
            <th style="text-align:center;">Атрибут</th>
            <th style="text-align:center;">Индекс</th>
            <th style="text-align:center;">Тип</th>
            <th style="text-align:center;">Длина</th>
            <th style="text-align:center;">>НЧ длины</th>
            <th style="text-align:center;">Мощность</th>
            <th style="text-align:center;">НЧ мощности</th>
            <th style="text-align:center;">Комментарий</th>
        </tr>
    </thead>
    <tbody>
    <tr class="rowForAdd">
        <td class="name" style="text-align:center;"><input type="text"/></td>
        <td class="index" style="text-align:center;"><input type="text"/></td>
        <td class="type" style="text-align:center;"><input type="text"/></td>
        <td class="length" style="text-align:center;"><input type="text"/></td>"
        <td class="length_fuzzyNumber" style="text-align:center;"><input type="text"/></td>
        <td class="power" style="text-align:center;"><input type="text"/></td>
        <td class="namepower_fuzzyNumber" style="text-align:center;"><input type="text"/></td>
        <td class="comment" style="text-align:center;"><input type="text"/></td>
    </tr>
    </tbody>
</table>

Где-то есть кнопка, на неё обработчик такой:

function saveAll() {
    $('#attributes tr.rowForAdd').each(function () {
        $.each(this.cells, function (obj) {
            alert($(this).find('input').val());
        });
    });
}

Будут появляться алерты с содержимым каждой ячейки.