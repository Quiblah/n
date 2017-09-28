## example 1

bA|0|1
---|---|---
0|1|0
1|1|0

x = A`.B` + A`.B

= A`
## example 2

bA|0|1
---|---|---
0|0|1
1|1|1

X = A.B` + A`.B + A.B

= A.B` + B.(A`+A)

= A.B` + B

= A+B

## Example 3

cBA|00|01|11|10
---|---|---|---|---
0|0|0|0|0
1|1|1|1|1

X = A`.B`.C + A.B`.C + A.B.C + A`.B.C

= B`.C.(A`+A) + B.C.(A+A`)

= C

## Example 4
Use a karnaugh map to minimise the following expresssion:

X = A.B`.C + A`.B.C + A`.B`.C + A`.B`.C` + A.B`.C`

cBA|00|01|11|10
---|---|---|---|---
0|1|1|0|0
1|1|1|0|1

X = B` + A`.C