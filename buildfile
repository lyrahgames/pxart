./: tests/ examples/ benchmarks/ manifest

./: lib{pxart}: pxart/hxx{**}
{
  cxx.export.poptions = "-I$out_root" "-I$src_root"
}
hxx{**}: install.subdirs = true

tests/: install = false
examples/: install = false
benchmarks/: install = false