## TeX

<!-- MarkdownTOC -->

- [Manually add new package](#manually-add-new-package)
- [Flatten a project into a single file](#flatten-a-project-into-a-single-file)
- [latexdiff](#latexdiff)
    - [Using filter to adjust output](#using-filter-to-adjust-output)

<!-- /MarkdownTOC -->

### Manually add new package

Let's say you have `duomasterforside.sty` and you want to make it available in all your documents. Just in case, check if it is already in the system:

``` sh
$ kpsewhich duomasterforside.sty
```

If that returns nothing, then you need to add it to your TDS (*TeX Directory Structure*). Find TeX home directory:

``` sh
$ kpsewhich -var-value=TEXMFHOME
```

Though keep in mind that it might give you a wrong path (*or rather a symlinked one*). If anything, on Mac OS the right one is `/usr/local/texlive/texmf-local/tex/latex/local`.

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

### Flatten a project into a single file

<https://tex.stackexchange.com/a/21840/80037>

``` sh
$ which latexpand
/Library/TeX/texbin/latexpand

$ mkdir ../flat
$ latexpand ./_main.tex > ../flat/_main.tex
```

### latexdiff

When you need show some changes for/from reviewer in your final PDF, you can use [git-latexdiff](https://ctan.org/pkg/git-latexdiff) script to visualize the `git diff` output. You likely already have it installed in your system together with the rest of your TeX distribution:

``` sh
$ git latexdiff --version
$ git latexdiff -h
```

For example, having the following (*uncommitted*) changes on top of [this revision](https://github.com/retifrav/tex-sandbox/tree/e6958a4dbb04a4127e35730ef803851a2e75ab0f):

``` patch
diff --git a/1-intro.tex b/1-intro.tex
index c997c47..96a5c27 100644
--- a/1-intro.tex
+++ b/1-intro.tex
@@ -87,7 +87,7 @@ Examples: The citation may be made in any of several ways, depending on the natu
 
 The Finnmark carbonate platform consists of gently north dipping strata ... (Bugge 1995).
 
-When a work has two or three authors, use the surnames of both authors in all citations. Join the two names by the word "and", like (Bugge, Nilsen and Sogge 1995). When a work has 4-6 authors, use the surnames of all authors in the first citation. In subsequent citations, include only the surname of the first author followed by "et al.": (Bugge et al. 1995).
+When a work has three or four authors, use the surnames of both authors in all citations. Join the two names by the word "and", like (Bugge, Nilsen and Sogge 1995). When a work has 4-6 authors, use the surnames of all authors in the first citation. In subsequent citations, include only the surname of the first author followed by "et al.": (Bugge et al. 1995).
 
 \subsubsection{Reference list}
```

you can render them to PDF like this:

``` sh
$ git latexdiff --main ./_main.tex -o ./diff.pdf HEAD --
```

here:

- `HEAD` - "old" state of the document/project, and instead of `HEAD` here can be a specific commit hash;
- `--` - "new" state of the document/project, `--` means current working directory, but it can be also a specific commit hash.

And the result will be:

![](./img/latexdiff.png)

If you get errors about files not found, such as images, then check if you have them or their extensions blacklisted in `.gitignore`; same goes for failing bibliography - `*.bbl` files need to be tracked.

If you still get errors about missing files, check if they are untracked, and if they are, than just staging them should resolve the problem. That is probably an error of some sort within the `git-latexdiff` script.

One other thing you can try is running `git-latexdiff` with either `--whole-tree` or (*/and?*) `--latexdiff-flatten`, or try a different engine/backend, such as `--latexmk`.

#### Using filter to adjust output

If `latexdiff` fails with your project, chances are you have an incorrect TeX documents, which nevertheless (*barely*) pass the usual PDF generation, but fail for good after `latexdiff` adds its commands there (*like `\DIFaddbeginFL` and others*).

Errors you might be getting:

```
! Misplaced \noalign.
\hline ->\noalign
                  {\ifnum 0=`}\fi \hrule \@height \arrayrulewidth \futurelet...
l.403     \DIFaddbeginFL \end{tabularx}
```

or:

```
! Misplaced \omit.
\multispan ->\omit
                   \@multispan
l.428 \end{tabularx}
```

or something else. But they all were about something around tables. Here's a [related bugreport](https://github.com/ftilmann/latexdiff/issues/5).

So one thing you can do is to get you ~~shit~~ tables in order and format them properly.

But if they are in perfect order, then you might need to adjust the TeX output of `latexdiff` before it gets to be process to generate the final PDF. For that exact purpose there is a `--filter` argument, in which you can provide a set of commands to perform after the TeX output is produced but before it gets to be compiled into final PDF.

For example:

``` sh
$ git latexdiff --main ./_main.tex -o ./diff.pdf 7c5f0f71bd8c8352f837282dae5116d06b19cc70 HEAD \
    --filter "echo '---'; pwd; cp ./_main.tex ~/Desktop/latexdiff-output.tex; echo '---'"
/Library/TeX/texbin/latexpand
---
/tmp/git-latexdiff.53212/new
---
This is pdfTeX, Version 3.141592653-2.6-1.40.24 (TeX Live 2022) (preloaded format=pdflatex)
 restricted \write18 enabled.
entering extended mode
(./_main.tex
...
```

As you can see, before `pdfTeX` was executed, the commands from `--filter` were executed, printing current working directory and copying `./_main.tex` file from it to your Desktop. Note that working directory - `/tmp/git-latexdiff.53212/new/` - this is where `latexdiff` puts its TeX output, which will be then processed into final PDF.

To fix/adjust things you can use `sed` (*`gsed` on Mac OS*), for example like this:

``` sh
$ git latexdiff --main ./_main.tex -o ./diff.pdf 7c5f0f71bd8c8352f837282dae5116d06b19cc70 HEAD \
    --filter "echo '---'; echo 'Fixing latexdiff output...'; gsed -i 's/\\\DIFaddbegin \\\midrule/\\\midrule \\\DIFaddbegin/g; s/\\\DIFaddend \\\bottomrule/\\\bottomrule \\\DIFaddend/g; s/\\\DIFdelend \\\bottomrule/\\\bottomrule \\\DIFdelend/g; s/%DIFDELCMD </%DIFDELCMD <\n/g;' ./_main.tex; echo '---'"
```

Again, note that `--main ./_main.tex` is your actual main project file, but `./_main.tex` inside `--filter` is the `latexdiff` output that is located inside its working folder (*`/tmp/git-latexdiff.53212/new/` or some other*), so it's not the same file, although of course their names are the same.

If we are talking about replacing things in files here, then you might want to [flatten](#flatten-a-project-into-a-single-file) your project into a single file. And of course you'll need to do that for both revisions which you'd like to compare. Or perhaps you can try to make `latexdiff` do that for you, as it seems to have some arguments related to flattening.
