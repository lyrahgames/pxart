int main(int argc, char** argv) {
  if (2 != argc) {
    cout << "usage:\n" << argv[0] << " <number of elements>\n";
    return -1;
  }
  stringstream input{argv[1]};
  uint64_t sample_count;
  input >> sample_count;

  random_device rd{};
  benchmark{sample_count}  //
      .run<uint32_t>("std mt  ", std::mt19937{rd()})
      .run("boost mt", boost::random::mt19937{rd()})
      .separate()
      .run("mt19937 ", pxart::mt19937{rd})
      .run(".simd256", pxart::simd256::mt19937{rd})
      .run(".simd128", pxart::simd128::mt19937{rd})
      .separate()
      .run("xrsr128p", pxart::xrsr128p{rd})
      .run("     .x2", pxart::xrsr128p{rd}, pxart::xrsr128p{rd})
      .run(".simd256", pxart::simd256::xrsr128p{rd})
      .run("    ..x2", pxart::simd256::xrsr128p{rd},
           pxart::simd256::xrsr128p{rd})
      .run(".simd128", pxart::simd128::xrsr128p{rd})
      .run("    ..x2", pxart::simd128::xrsr128p{rd},
           pxart::simd128::xrsr128p{rd})
      .separate()
      .run("msws    ", pxart::msws{rd})
      .run("     .x2", pxart::msws{rd}, pxart::msws{rd})
      .run(".simd256", pxart::simd256::msws{rd})
      .run("    ..x2", pxart::simd256::msws{rd}, pxart::simd256::msws{rd})
      .run(".simd128", pxart::simd128::msws{rd})
      .run("    ..x2", pxart::simd128::msws{rd}, pxart::simd128::msws{rd});
}