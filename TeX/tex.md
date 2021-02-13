## TeX

<!-- MarkdownTOC -->

- [Manually add new package](#manually-add-new-package)

<!-- /MarkdownTOC -->

### Manually add new package

Let's say you have `duomasterforside.sty` and you want to make it available in all your documents. Just in case, check if it is already in the system:

``` sh
$ kpsewhich duomasterforside.sty
```

If that returns nothing, then you need to add it to your TDS (*TeX Directory Structure*.) Find TeX home directory:

``` sh
$ kpsewhich -var-value=TEXMFHOME
```

Though keep in mind it might give a wrong path. If anything, on Mac OS it's `/usr/local/texlive/texmf-local/tex/latex/local`.

Just copy a folder with your package/styles/etc there and re-hash the database:

``` sh
$ sudo texhash
texhash: Updating /usr/local/texlive/2020/texmf-config/ls-R...
texhash: Updating /usr/local/texlive/2020/texmf-dist/ls-R...
texhash: Updating /usr/local/texlive/2020/texmf-var/ls-R...
texhash: Updating /usr/local/texlive/texmf-local/ls-R...
texhash: Done.
```

Now the package should be available:

``` sh
$ kpsewhich duomasterforside.sty
/usr/local/texlive/texmf-local/tex/latex/local/duoforside/duomasterforside.sty
```

And you can use it in your documents:

``` tex
\usepackage{babel,duomasterforside}
```
