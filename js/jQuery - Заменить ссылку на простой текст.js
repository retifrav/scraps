<script type="text/javascript">

    $(document).ready(function () {
        $("#main_link").replaceWith(function () {
            return $("<span style=\"font-size:20px;\"><strong>"
                + $(this).html() + "</strong></span>");
        });
    });

</script>