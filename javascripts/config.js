window.MathJax = {
  tex: {
    inlineMath: [
      ['\\(', '\\)']
    ],
    displayMath: [
      ['\\[', '\\]']
    ],
    processEscapes: true,
    processEnvironments: true
  },
  options: { ignoreHtmlClass: '.*|', processHtmlClass: 'arithmatex' }

};

MathJax.Hub.Config({ jax: ['input/TeX', 'output/HTML-CSS'], displayAlign: 'left' });