README_color_traversal_moves.txt

state = (sign, direction, side, xi)
value = (xi', change_direction, change_side)


sign - crossing sign;
	   "+"" = 1, "-" = -1;
direction - FORWARD or BACKWARD (in respect to crossing orientation);
			"FORWARD" = 1, "BACKWARD" = -1;
side - wchich side of the crossing segment are we at (left/right);
	   left = -1, right = 1;
xi - on which crossing segment are we at (x1, x2, x3, x4);
	 "x1" = 0, "x2" = 1, "x3" = 2, "x4" = 3














Map:

// POSITIVE CROSSING
//forward, right
(1,1,1,0)	{1,1,1} 
(1,1,1,1)	{-1,1,1} 
(1,1,1,2)	{-1,1,1}
(1,1,1,3)	{0,-1,-1}

//forward, left
(1,1,-1,0)	{3,-1,-1}
(1,1,-1,1)	{-1, 1, 1} 
(1,1,-1,2)	{-1, 1, 1} 
(1,1,-1,3)	{2, 1, 1}

//backward, right
(1,-1,1,0)	{-1,1,1}
(1,-1,1,1)	{0,1,1}
(1,-1,1,2)	{3,-1,-1}
(1,-1,1,3)	{-1,1,1}

//backward, left
(1,-1,-1,0)	{-1,1,1}
(1,-1,-1,1)	{2,-1,-1}
(1,-1,-1,2)	{3,1,1}
(1,-1,-1,3)	{-1,1,1}


//NEGATIVE CROSSING
//forward, right
(-1,1,1,0)	{1,-1,-1}
(-1,1,1,1)	{2,1,1}
(-1,1,1,2)	{-1,1,1}
(-1,1,1,3)	{-1,1,1}

//forward, left
(-1,1,-1,0)	{3,1,1}
(-1,1,-1,1)	{0,-1,-1}
(-1,1,-1,2)	{-1,1,1}
(-1,1,-1,3)	{-1,1,1}

//backward, right
(-1,-1,1,0)	{-1,1,1}
(-1,-1,1,1)	{-1,1,1}
(-1,-1,1,2)	{1,1,1}
(-1,-1,1,3)	{2,-1,-1}

//backward, left
(-1,-1,-1,0)	{-1,1,1}
(-1,-1,-1,1)	{-1,1,1}
(-1,-1,-1,2)	{3,-1,-1}
(-1,-1,-1,3)	{0,1,1}