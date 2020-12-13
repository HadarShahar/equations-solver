# equations-solver
Linear equations solver - solves N equations of N different variables!
It reads the equations from the file EQUATIONS_FILENAME.

## Sample input:
1.5(X+Y+Z) = 3Z 
-(Y+Z) + 10 = 5
2(X+Y) - 4Z = -6


## Sample output:
Reading 3 equations...
Parsing equation (1): '1.5(X+Y+Z) = 3Z '
Solving for: X
+1.50X +1.50Y +1.50Z  = +3.00Z
+1.50X  = -1.50Y +1.50Z
+X  = -1.00Y +Z

Parsing equation (2): '-(Y+Z) + 10 = 5'
Solving for: Y
-1.00Y -1.00Z +10.00  = +5.00
-1.00Y  = +Z -5.00
+Y  = -1.00Z +5.00

Parsing equation (3): '2(X+Y) - 4Z = -6'
Substituing equation (1) in equation (3)
Substituing equation (2) in equation (3)
Solving for: Z
+2.00Z -10.00 +2.00Z -2.00Z +10.00 -4.00Z  = -6.00
-2.00Z  = -6.00
+Z  = +3.00

Substituing equation (2) in equation (1)
Substituing equation (3) in equation (1)
Solving for: X
+X  = +3.00 -5.00 +3.00
+X  = +1.00

Substituing equation (3) in equation (2)
Solving for: Y
+Y  = -3.00 +5.00
+Y  = +2.00


================================================
Final answer:
+X  = +1.00
+Y  = +2.00
+Z  = +3.00
