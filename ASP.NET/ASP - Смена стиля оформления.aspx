<head>

...

<link id="bootstrapCSS" href="lib/bootstrap/css/bootstrap.min.css" rel="stylesheet" />

...

</head>

<body>

...

<li class="dropdown">
<a href="#" class="dropdown-toggle" data-toggle="dropdown"><span class="glyphicon glyphicon-picture"></span> Стиль <b class="caret"></b></a>
    <ul id = "ul_styles">
        <li><b>Стандартные</b></li>
        <li style="text-align: center;"><a href="#" class="dropdown-item-uppz" rel="../lib/bootstrap/css/bootstrap.min.css">Default</a></li>
        <li><b>Дополнительные</b></li>
        <li style="text-align: center;"><a href="#" class="dropdown-item-uppz" rel="../lib/bootstrap/css/cyborg.css">Cyborg</a></li>
        <li style="text-align: center;"><a href="#" class="dropdown-item-uppz" rel="../lib/bootstrap/css/slate.css">Slate</a></li>
        <li style="text-align: center;"><a href="#" class="dropdown-item-uppz" rel="../lib/bootstrap/css/cupid.css">Cupid</a></li>
        <li style="text-align: center;"><a href="#" class="dropdown-item-uppz" rel="../lib/bootstrap/css/superhero.css">Superhero</a></li>
    </ul>
</li>

...

<script type="text/javascript">

    $(document).ready(function () {

        // установка выбранного стиля из куки
        if ($.cookie("nameCSS")) {
            $("#bootstrapCSS").attr("href", $.cookie("nameCSS"));
        }
        // сохранение выбранного стиля в куки
        $("#ul_styles li a").click(function () {
            $("#bootstrapCSS").attr("href", $(this).attr('rel'));
            $.cookie("nameCSS", $(this).attr('rel'), { expires: 365, path: '/' });
            return false;
        });

    });

</script>

</body>