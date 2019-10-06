void drawGrapherLine(drawData drawInformation) {
	Uint32 * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	windowSection WINDOW = *(drawInformation.window);

	int x1 = drawInformation.arguments[0] + WINDOW.X1;
	int y1 = drawInformation.arguments[1] + WINDOW.Y1;
	int x2 = drawInformation.arguments[2] + WINDOW.X1;
	int y2 = drawInformation.arguments[3] + WINDOW.Y1;
	int color = drawInformation.arguments[4];

	int X1,Y1,X2,Y2;			// creates new coordinates that are sorted by y value
	X1 = x1; Y1 = y1;
	X2 = x2; Y2 = y2;
	if (y1 > y2) {
		X1 = x2; Y1 = y2;
		X2 = x1; Y2 = y1;
	}

	int xdiff = X2 - X1;			// delta x and delta y values for drawing the line
	int ydiff = Y2 - Y1;
	int j = 0;				// the y value counter
	int yLimit = ydiff;			// the limit for the y value counter

	if (xdiff == 0 && ydiff == 0) {
		int i1 = X1 - 1;
		int i2 = X1 + 1;
		int j1 = Y1 - 1;
		int j2 = Y1 + 1;
		if (i1 < WINDOW.X1) { i1 = WINDOW.X1; }
		if (j1 < WINDOW.Y1) { j1 = WINDOW.Y1; }
		if (i2 >= WINDOW.X2) { i2 = WINDOW.X2 - 1; }
		if (j2 >= WINDOW.Y2) { j2 = WINDOW.Y2 - 1; }

		int k,l;
		for ( k = i1; k < i2; k++ ) { for ( l = j1; l < j2; l++ ) {
			pixels[WINDOW_WIDTH * l + k] = color;
		}}
		
		return;
	}
	if (yLimit + Y1 >= WINDOW.Y2) { yLimit = WINDOW.Y2 - Y1 - 1; } //Prevent screen overflow
	if (Y1 < WINDOW.Y1) { j = -Y1 + WINDOW.Y1; }
	if (abs(xdiff) < ydiff) {		// if more vertical, one pixel per row
		int i;

		while (j < yLimit) {

			i = j * xdiff / ydiff + X1;

			int i1 = i - 1;
			int i2 = i + 1;
			int j1 = Y1 + j - 1;
			int j2 = Y1 + j + 1;
			if (i1 < WINDOW.X1) { i1 = WINDOW.X1; }
			if (j1 < WINDOW.Y1) { j1 = WINDOW.Y1; }
			if (i2 >= WINDOW.X2) { i2 = WINDOW.X2 - 1; }
			if (j2 >= WINDOW.Y2) { j2 = WINDOW.Y2 - 1; }

			int k,l;
			for ( k = i1; k < i2; k++ ) { for ( l = j1; l < j2; l++ ) {
				pixels[WINDOW_WIDTH * l + k] = color;
			}}

			//if (i < WINDOW.X2 && i >= WINDOW.X1) {pixels[WINDOW_WIDTH * (Y1 + j) + i] = color;}
			j++;
		}
	}
	else {
		int drawMemStart, drawMemEnd, I1, I2;
		if (ydiff == 0) {				//horizontal line

			if (X1 > X2) { int tmp = X1; X1 = X2; X2 = tmp; }

			int i1 = X1 - 1;
			int i2 = X2 + 1;
			int j1 = Y1 + j - 1;
			int j2 = Y2 + j + 1;
			if (i1 < WINDOW.X1) { i1 = WINDOW.X1; }
			if (j1 < WINDOW.Y1) { j1 = WINDOW.Y1; }
			if (i2 >= WINDOW.X2) { i2 = WINDOW.X2 - 1; }
			if (j2 >= WINDOW.Y2) { j2 = WINDOW.Y2 - 1; }

			int k,l;
			for ( k = i1; k < i2; k++ ) { for ( l = j1; l < j2; l++ ) {
				pixels[WINDOW_WIDTH * l + k] = color;
			}}
		}
		else {
			I2 = X1;
			while (j < yLimit) {
				I1 = I2;
				I2 = X1 + (j+1) * xdiff / ydiff;

				int i1,i2;
				if (xdiff < 0) {
					i1 = I2 - 1;
					i2 = I1 + 1;
				}
				else {
					i1 = I1 - 1;
					i2 = I2 + 1;
				}
				int j1 = Y1 + j - 1;
				int j2 = Y1 + j + 1;
				if (i1 < WINDOW.X1) { i1 = WINDOW.X1; }
				if (j1 < WINDOW.Y1) { j1 = WINDOW.Y1; }
				if (i2 >= WINDOW.X2) { i2 = WINDOW.X2 - 1; }
				if (j2 >= WINDOW.Y2) { j2 = WINDOW.Y2 - 1; }

				int k,l;
				for ( k = i1; k < i2; k++ ) { for ( l = j1; l < j2; l++ ) {
					pixels[WINDOW_WIDTH * l + k] = color;
				}}

				j++;
			}
		}
	}
}

void grapherLine(int x1, int y1, int x2, int y2, int color, windowSection * window, screenData * screen ) {
	int arguments[5] = {x1, y1, x2, y2, color};
	addEventExecute(drawGrapherLine, 3, arguments, NULL, window, 5, screen );
}

void drawCurve( drawData drawInfo ) {
	double bdist = 10 * dist;
	curveData * curve = (curveData*)(drawInfo.dataPointer);

	drawData lineData;
	lineData.arguments[4] = drawInfo.arguments[2];
	lineData.screen = drawInfo.screen;
	lineData.window = drawInfo.window;

	double width2 = (double)(drawInfo.arguments[0])/2;
	double height2 = (double)(drawInfo.arguments[1])/2;

	double startX = curve -> startX;
	double endX = curve -> endX;

	if (startX == endX) {return;}

	double nSteps = curve -> nSteps;
	double step = (endX - startX)/((double)(nSteps));
	double T = startX; predefVariables[0].val = T;

	double t;
	double x = evalMathExpression( curve -> x );
	double y = evalMathExpression( curve -> y );
	double z = evalMathExpression( curve -> z );

	t = ( d + (vx*x + vy*y + vz*z) ) / (vx*vx + vy*vy + vz*vz);
	x += t * vx; y += t * vy; z += t * vz;
	lineData.arguments[2] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
	lineData.arguments[3] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;

	bool point1IsIn = false;
	bool point2IsIn = false;
	if ( abs(x-cx) < bdist && abs(y-cy) < bdist && abs(z-cz) < bdist ) { point1IsIn = true; }

	int i;
	for ( i = 0; i < nSteps; i++ ) {
		T += step;
		predefVariables[0].val = T;
		x = evalMathExpression( curve -> x );
		y = evalMathExpression( curve -> y );
		z = evalMathExpression( curve -> z );

		point2IsIn = point1IsIn;
		point1IsIn = false;
		if ( abs(x-cx) < bdist && abs(y-cy) < bdist && abs(z-cz) < bdist ) { point1IsIn = true; }

		t = ( d + (vx*x + vy*y + vz*z) ) / (vx*vx + vy*vy + vz*vz);
		x += t * vx; y += t * vy; z += t * vz;
		lineData.arguments[0] = lineData.arguments[2];
		lineData.arguments[1] = lineData.arguments[3];
		lineData.arguments[2] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
		lineData.arguments[3] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;

		if ( point2IsIn && point1IsIn ) {drawGrapherLine( lineData );}
	}
}

void drawParam( drawData drawInfo ) {
	paramData * param = (paramData*)(drawInfo.dataPointer);

	drawData lineData;
	lineData.arguments[4] = drawInfo.arguments[2];
	lineData.screen = drawInfo.screen;
	lineData.window = drawInfo.window;

	double width = drawInfo.arguments[0];
	double height = drawInfo.arguments[1];
	double scale = 1 / pixScale;
	int color = drawInfo.arguments[2];

	double startX = param -> startX;
	double endX = param -> endX;

	if (startX == endX) {return;}

	double nSteps = param -> nSteps;
	double step = (endX - startX)/((double)(nSteps));
	double T = startX; predefVariables[0].val = T;

	double t;
	double x = evalMathExpression( param -> x );
	double y = evalMathExpression( param -> y );

	lineData.arguments[2] = width / 2 + ( x - gXCenter ) * pixScale;
	lineData.arguments[3] = height / 2 + ( gYCenter - y ) * pixScale;

	int i;
	for ( i = 0; i < nSteps; i++ ) {
		T += step;
		predefVariables[0].val = T;
		x = evalMathExpression( param -> x );
		y = evalMathExpression( param -> y );

		lineData.arguments[0] = lineData.arguments[2];
		lineData.arguments[1] = lineData.arguments[3];
		lineData.arguments[2] = width / 2 + ( x - gXCenter ) * pixScale;
		lineData.arguments[3] = height / 2 + ( gYCenter - y ) * pixScale;

		drawGrapherLine( lineData );
	}
}

void drawPoint2D( drawData drawInfo ) {
	pointData * point = (pointData*)(drawInfo.dataPointer);

	drawData pointData;
	pointData.arguments[3] = drawInfo.arguments[2];
	pointData.screen = drawInfo.screen;
	pointData.window = drawInfo.window;

	double width = drawInfo.arguments[0];
	double height = drawInfo.arguments[1];
	double scale = 1 / pixScale;
	int color = drawInfo.arguments[2];

	double x = point -> x;
	double y = point -> y;

	pointData.arguments[0] = width / 2 + ( x - gXCenter ) * pixScale;
	pointData.arguments[1] = height / 2 + ( gYCenter - y ) * pixScale;
	pointData.arguments[2] = 7;
	drawCircle( pointData );
}

void drawPoint3D( drawData drawInfo ) {
	pointData * point = (pointData*)(drawInfo.dataPointer);

	drawData pointData;
	pointData.arguments[3] = drawInfo.arguments[2];
	pointData.screen = drawInfo.screen;
	pointData.window = drawInfo.window;

	double width2 = drawInfo.arguments[0] / 2;
	double height2 = drawInfo.arguments[1] / 2;
	int color = drawInfo.arguments[2];

	double x = point -> x;
	double y = point -> y;
	double z = point -> z;

	double bdist = 10 * dist;
	if ( abs(x-cx) < bdist && abs(y-cy) < bdist && abs(z-cz) < bdist ) {
		double t = ( d + (vx*x + vy*y + vz*z) ) / (vx*vx + vy*vy + vz*vz);
		x += t * vx; y += t * vy; z += t * vz;

		pointData.arguments[0] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
		pointData.arguments[1] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
		pointData.arguments[2] = 7;
		drawCircle( pointData );
	}
}

void draw2DExpression( drawData drawInformation ) {
	mathExpression * function = (mathExpression*)(drawInformation.dataPointer);

	Uint32 * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;

	int width = drawInformation.arguments[0];
	int height = drawInformation.arguments[1];
	double scale = 1 / pixScale;
	int color = drawInformation.arguments[2];

	int Xbase = drawInformation.window -> x;
	int Ybase = drawInformation.window -> y;
	int X1 = drawInformation.window -> X1;
	int X2 = drawInformation.window -> X2;
	int Y1 = drawInformation.window -> Y1;
	int Y2 = drawInformation.window -> Y2;

	int thickness = 1;

	int i, i1, i2, j1, j2;
	int imax = drawInformation.arguments[0];
	predefVariables[0].val = gXCenter - scale * width / 2 ;
	int jold = ( gYCenter - evalMathExpression( function ) ) * pixScale + height / 2;
	for ( i = 0; i < imax; i++ ) {
		double x = (double)(i - width/2) * scale + gXCenter;
		predefVariables[0].val = x;
		double y =  evalMathExpression( function );
		int j = height / 2 + ( gYCenter - y ) * pixScale;
		if ( j == jold ) {
			i1 = Xbase + i - 1; j1 = Ybase + jold - thickness; i2 = i1 + 1 + thickness; j2 = j1 + 1 + thickness;
		}
		else if ( j < jold ) {
			i1 = Xbase + i - 1; j1 = Ybase + j - thickness; i2 = i1 + 1 + thickness; j2 = j1 + jold - j + thickness;
		}
		else {
			i1 = Xbase + i - 1; j1 = Ybase + jold - thickness; i2 = i1 + 1 + thickness; j2 = j1 + j - jold + thickness;
		}
		jold = j;

		if ( i1 < X1 ) { i1 = X1; }
		if ( j1 < Y1 ) { j1 = Y1; }
		if ( i2 > X2 ) { i2 = X2; }
		if ( j2 > Y2 ) { j2 = Y2; }

		int k, l;
		for ( k = i1; k < i2; k++ ) { for ( l = j1; l < j2; l++ ) {
			pixels[ WINDOW_WIDTH * l + k ] = color;
		}}
	}
}

void drawVector( drawData drawInfo ) {}

void draw3DExpression( drawData drawInfo ) {
	double bdist = 10 * dist;
	mathExpression * function = (mathExpression*)(drawInfo.dataPointer);

	drawData lineData;
	lineData.arguments[4] = drawInfo.arguments[2];
	lineData.screen = drawInfo.screen;
	lineData.window = drawInfo.window;

	double width2 = (double)(drawInfo.arguments[0])/2;
	double height2 = (double)(drawInfo.arguments[1])/2;

	double nSteps = 100;
	double step = (double)(20) * dist / nSteps;

	double x,y,z, X,Y,Z, t;

		//cout << "|################################################################" << endl;

	int j;
	for ( j = -10; j <= 10; j++ ) {
		X = -dist * 10; predefVariables[0].val = X;
		Y = ((double)(j))*dist; predefVariables[1].val = Y;
		Z = evalMathExpression( function );

		t = ( d + (vx*X + vy*Y + vz*Z) ) / (vx*vx + vy*vy + vz*vz);
		x = X + t * vx; y = Y + t * vy; z = Z + t * vz;
		lineData.arguments[2] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
		lineData.arguments[3] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;

		bool point1IsIn = false;
		bool point2IsIn = false;
		if ( abs(Z-cz) < bdist ) { point1IsIn = true; }

		int Zold;
		int i;
		for ( i = 0; i < nSteps; i++ ) {
			Zold = Z;

			X += step;
			predefVariables[0].val = X;
			Z = evalMathExpression( function );

			point2IsIn = point1IsIn;
			point1IsIn = false;
			if ( abs(Z-cz) <= bdist ) { point1IsIn = true; }

			t = ( d + (vx*X + vy*Y + vz*Z) ) / (vx*vx + vy*vy + vz*vz);
			x = X + t * vx; y = Y + t * vy; z = Z + t * vz;
			lineData.arguments[0] = lineData.arguments[2];
			lineData.arguments[1] = lineData.arguments[3];
			lineData.arguments[2] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
			lineData.arguments[3] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;

			//drawLine( lineData );
			if ( point2IsIn && point1IsIn ) {drawGrapherLine( lineData );}
			/*else if ( point2IsIn ) {						//Zold is in
				if (Z - cz >= bdist) {
					double Xnew = X - step + (bdist-Zold)/(Z-Zold)*step;
					cout << Xnew << "\t" << Y << "\t" << bdist << "\t1" << endl;
					t = ( d + (vx*Xnew + vy*Y + vz*bdist) ) / (vx*vx + vy*vy + vz*vz);
					x = Xnew + t * vx; y = Y + t * vy; z = bdist + t * vz;
					lineData.arguments[2] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
					lineData.arguments[3] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
					drawGrapherLine( lineData );
				}
				else if (Z - cz <= -bdist) {
					double Xnew = X - step + (-bdist-Zold)/(Z-Zold)*step;
					cout << Xnew << "\t" << Y << "\t" << bdist << "\t1b" << endl;
					t = ( d + (vx*Xnew + vy*Y - vz*bdist) ) / (vx*vx + vy*vy + vz*vz);
					x = Xnew + t * vx; y = Y + t * vy; z = -bdist + t * vz;
					lineData.arguments[2] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
					lineData.arguments[3] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
					//drawGrapherLine( lineData );
				}
			}
			else if ( point1IsIn ) {
				if (Zold - cz >= bdist) {
					double Xnew = X - step + (bdist-Z)/(Zold-Z)*step;
					cout << Xnew << "\t" << Y << "\t" << bdist << "\t2" << endl;
					t = ( d + (vx*Xnew + vy*Y + vz*bdist) ) / (vx*vx + vy*vy + vz*vz);
					x = Xnew + t * vx; y = Y + t * vy; z = bdist + t * vz;
					lineData.arguments[0] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
					lineData.arguments[1] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
					//drawGrapherLine( lineData );
				}
				else if (Zold - cz <= -bdist) {
					double Xnew = X - step + (-bdist-Z)/(Zold-Z)*step;
					cout << Xnew << "\t" << Y << "\t" << bdist << "\t2b" << endl;
					t = ( d + (vx*Xnew + vy*Y - vz*bdist) ) / (vx*vx + vy*vy + vz*vz);
					x = Xnew + t * vx; y = Y + t * vy; z = -bdist + t * vz;
					lineData.arguments[0] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
					lineData.arguments[1] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
					//drawGrapherLine( lineData );
				}
			}*/
		}
	}

	j;
	for ( j = -10; j <= 10; j++ ) {
		X = ((double)(j))*dist; predefVariables[0].val = X;
		Y = -dist * 10; predefVariables[1].val = Y;
		Z = evalMathExpression( function );

		t = ( d + (vx*X + vy*Y + vz*Z) ) / (vx*vx + vy*vy + vz*vz);
		x = X + t * vx; y = Y + t * vy; z = Z + t * vz;
		lineData.arguments[2] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
		lineData.arguments[3] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;

		bool point1IsIn = false;
		bool point2IsIn = false;
		if ( abs(Z-cz) < bdist ) { point1IsIn = true; }

		int Zold;
		int i;
		for ( i = 0; i < nSteps; i++ ) {
			Zold = Z;

			Y += step;
			predefVariables[1].val = Y;
			Z = evalMathExpression( function );

			point2IsIn = point1IsIn;
			point1IsIn = false;
			if ( abs(Z-cz) < bdist ) { point1IsIn = true; }

			t = ( d + (vx*X + vy*Y + vz*Z) ) / (vx*vx + vy*vy + vz*vz);
			x = X + t * vx; y = Y + t * vy; z = Z + t * vz;
			lineData.arguments[0] = lineData.arguments[2];
			lineData.arguments[1] = lineData.arguments[3];
			lineData.arguments[2] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
			lineData.arguments[3] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;

			//drawLine( lineData );
			if ( point2IsIn && point1IsIn ) {drawGrapherLine( lineData );}
			/*else if ( point2IsIn ) {
				if (Z - cz >= bdist) {
					//double Xnew = X;// - (Zold-10)/(Z-Zold)*step;
					cout << X << "\t" << Y << "\t" << bdist << "\t1" << endl;
					t = ( d + (vx*X + vy*Y + vz*bdist) ) / (vx*vx + vy*vy + vz*vz);
					x = X + t * vx; y = Y + t * vy; z = bdist + t * vz;
					lineData.arguments[2] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
					lineData.arguments[3] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
					//drawGrapherLine( lineData );
				}
				else if (Z - cz <= -bdist) {
					//double X = X-step;// - (Zold+10)/(Z-Zold)*step;
					cout << X << "\t" << Y << "\t" << bdist << "\t1b" << endl;
					t = ( d + (vx*X + vy*Y - vz*bdist) ) / (vx*vx + vy*vy + vz*vz);
					x = X + t * vx; y = Y + t * vy; z = -bdist + t * vz;
					lineData.arguments[2] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
					lineData.arguments[3] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
					//drawGrapherLine( lineData );
				}
			}
			else if ( point1IsIn ) {
				if (Zold - cz >= bdist) {
					//double Xnew = X;// - (Z-10)/(Z-Zold)*step;
					cout << X << "\t" << Y << "\t" << bdist << "\t2" << endl;
					t = ( d + (vx*X + vy*Y + vz*bdist) ) / (vx*vx + vy*vy + vz*vz);
					x = X + t * vx; y = Y + t * vy; z = bdist + t * vz;
					lineData.arguments[0] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
					lineData.arguments[1] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
					//drawGrapherLine( lineData );
				}
				else if (Zold - cz <= -bdist) {
					//double Xnew = X;// - (Z+10)/(Z-Zold)*step;
					cout << X << "\t" << Y << "\t" << bdist << "\t2b" << endl;
					t = ( d + (vx*X + vy*Y - vz*bdist) ) / (vx*vx + vy*vy + vz*vz);
					x = X + t * vx; y = Y + t * vy; z = -bdist + t * vz;
					lineData.arguments[0] = width2 + ((x - cx) * xVecx + (y - cy) * xVecy + (z - cz) * xVecz) * scale / dist;
					lineData.arguments[1] = height2 - ((x - cx) * yVecx + (y - cy) * yVecy + (z - cz) * yVecz) * scale / dist;
					//drawGrapherLine( lineData );
				}
			}*/
		}
	}
}

void printFloat( double value, int x, int y, char * textBuffer, int maxLen, drawData * window ) {
	int mag = floor( log( abs(value) ) / log ( 10 ) );
	float coef = value / pow( 10, mag );

	int digit = 6;
	textBuffer[ digit ] = 48 + abs(mag);
	digit--;
	if (mag < 0) {
		textBuffer[ digit ] = 45;
		digit--;
	}
	textBuffer[ digit ] = 101;

	if ( coef < 0 ) {
		textBuffer[0] = 45;
		coef *= -1;

		textBuffer[ 1 ] = 48 + (int)(coef);
		coef -= floor( coef );
		coef *= 10;
		textBuffer[ 2 ] = 46;

		int i;
		for ( i = 3; i < digit; i++ ) {
			textBuffer[ i ] = 48 + (int)(coef);
			coef -= floor( coef );
			coef *= 10;
		}
	}
	else {
		textBuffer[ 0 ] = 48 + (int)(coef);
		coef -= floor( coef );
		coef *= 10;
		textBuffer[ 1 ] = 46;

		int i;
		for ( i = 2; i < digit; i++ ) {
			textBuffer[ i ] = 48 + (int)(coef);
			coef -= floor( coef );
			coef *= 10;
		}
	}

	textBuffer[ 7 ] = 0;
	Text( textBuffer, x, y, 100, 1, 0, 0, window -> window, onScreen );
}
