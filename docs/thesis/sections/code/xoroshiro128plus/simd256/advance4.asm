vmovdqa ymm0, YMMWORD PTR [rsp]
vpxor   ymm1, ymm0, YMMWORD PTR [rsp+32]
lea     rdi, [rsp+80]
vpsrlq  ymm3, ymm0, 40
vpsllq  ymm2, ymm1, 16
vpsllq  ymm0, ymm0, 24
vpxor   ymm2, ymm2, ymm1
vpor    ymm0, ymm0, ymm3
vpsrlq  ymm3, ymm1, 27
vpxor   ymm2, ymm2, ymm0
vpsllq  ymm0, ymm1, 37
vpor    ymm0, ymm0, ymm3
vpsrlq  ymm1, ymm2, 40
vpxor   ymm0, ymm0, ymm2
vpsllq  ymm3, ymm2, 24
vpor    ymm3, ymm3, ymm1
vpsllq  ymm2, ymm0, 16
vpxor   ymm3, ymm3, ymm2
vpsllq  ymm1, ymm0, 37
vpsrlq  ymm2, ymm0, 27
vpxor   ymm3, ymm3, ymm0
vpor    ymm1, ymm1, ymm2
vpsrlq  ymm4, ymm3, 40
vpxor   ymm1, ymm1, ymm3
vpsllq  ymm2, ymm3, 24
vpsllq  ymm0, ymm1, 16
vpsrlq  ymm3, ymm1, 27
vpor    ymm2, ymm2, ymm4
vpxor   ymm2, ymm2, ymm0
vpsllq  ymm0, ymm1, 37
vpxor   ymm2, ymm2, ymm1
vpor    ymm0, ymm0, ymm3
vpxor   ymm0, ymm0, ymm2
vpsrlq  ymm3, ymm2, 40
vpsllq  ymm2, ymm2, 24
vpsllq  ymm1, ymm0, 16
vpor    ymm2, ymm2, ymm3
vpxor   ymm2, ymm2, ymm1
vpsrlq  ymm1, ymm0, 27
vpxor   ymm2, ymm2, ymm0
vpsllq  ymm0, ymm0, 37
vpor    ymm0, ymm0, ymm1
vmovdqa YMMWORD PTR [rsp], ymm2
vmovdqa YMMWORD PTR [rsp+32], ymm0
vzeroupper