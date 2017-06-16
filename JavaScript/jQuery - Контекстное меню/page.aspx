<div class="contextMenu" id="myMenu1">
    <ul>
    <li id="open">Open</li>
    <li id="email">Email</li>
    <li id="save">Save</li>
    <li id="close">Close</li>
    </ul>
    </div>
</div>

<input id="baton" type="button" value="Отправить"/>

<script type="text/javascript">
    $('#baton').contextMenu('myMenu1', {
            bindings: {
                'open': function (t) {
                    alert('Trigger was ' + t.id + '\nAction was Open');
                },
                'email': function (t) {
                    alert('Trigger was ' + t.id + '\nAction was Email');
                },
                'save': function (t) {
                    alert('Trigger was ' + t.id + '\nAction was Save');
                },
                'close': function (t) {
                    alert('Trigger was ' + t.id + '\nAction was Close');
                }
            }
        });
</script>