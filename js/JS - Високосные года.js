var str = '', i;

for ( i = 1970; i < 2050; i += 1 ) {
    if ( new Date( i, 1, 29 ).getMonth() === 1 ) {
        str += '<p>' + i + '</p>';
    }
}

document.body.innerHTML = str;