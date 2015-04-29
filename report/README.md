

# Paper

Basically each section is separated out into it's own file for easy navigation of the project.
This is done using the `\input` command which inserts a latex file inline to the location where it is called.




### Useful latex commands

This creates a subsection that is non-numbered.
If you wish to let them be numbered, do not include the `*`.
```latex
\subsection*{Introduction}
```



This creates a subsubsection that is non-numbered.
If you wish to let them be numbered, do not include the `*`.
```latex
\subsubsection*{Abstract}
```


This creates a citation for a `bibitem` named `columnar`.
```latex
\cite{columnar}
```




### Dependencies

Requires `latexmk` and `pdflatex` to be in your `PATH`.





