vmovdqa ymm1, YMMWORD PTR [rsp]
vpxor   ymm0, ymm1, YMMWORD PTR [rsp+32]
lea     rdi, [rsp+80]
vpsrlq  ymm3, ymm1, 40
vpsllq  ymm2, ymm0, 16
vpsllq  ymm1, ymm1, 24
vpxor   ymm2, ymm2, ymm0
vpor    ymm1, ymm1, ymm3
vpxor   ymm1, ymm2, ymm1
vmovdqa YMMWORD PTR [rsp], ymm1
vpsrlq  ymm1, ymm0, 27
vpsllq  ymm0, ymm0, 37
vpor    ymm0, ymm0, ymm1
vmovdqa YMMWORD PTR [rsp+32], ymm0
vzeroupper