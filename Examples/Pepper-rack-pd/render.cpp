// Calculate the centroid of the 1D sensors
    //-----------------------------------------

    g1D_1PositionWeighted = 0;
    g1D_1PositionTotal = 0;
    g1D_2PositionWeighted = 0;
    g1D_2PositionTotal = 0;
    g1D_3PositionWeighted = 0;
    g1D_3PositionTotal = 0;

    for(unsigned int j = 0; j < 26; j++) {
        g1D_1PositionWeighted += g1D_1Raw[j]*(j+1);
        g1D_1PositionTotal += g1D_1Raw[j];
        
        g1D_2PositionWeighted += g1D_2Raw[j]*(j+1);
        g1D_2PositionTotal += g1D_2Raw[j];
        
        g1D_3PositionWeighted += g1D_3Raw[j]*(j+1);
        g1D_3PositionTotal += g1D_3Raw[j];
    }
    
    if(g1D_1PositionTotal > 0){
        g1D_1Positions = g1D_1PositionWeighted/g1D_1PositionTotal;
    }
    
    if(g1D_2PositionTotal > 0){
        g1D_2Positions = g1D_2PositionWeighted/g1D_2PositionTotal;
    }
    
    if(g1D_3PositionTotal > 0){
        g1D_3Positions = g1D_3PositionWeighted/g1D_3PositionTotal;
    }
    //-----------------------------------------

    // Calculate the x + y position of the 2D sensor
    //-----------------------------------------
    g2D_PositionWeighted[0] = 0;
    g2D_PositionWeighted[1] = 0;
    g2D_PositionTotal[0] = 0;
    g2D_PositionTotal[1] = 0;

    for(unsigned int k = 0; k < 15; k++) {
        g2D_PositionWeighted[0] += g2DRawXAxis[k]*(k+1);
        g2D_PositionWeighted[1] += g2DRawYAxis[k]*(k+1);
        
        g2D_PositionTotal[0] += g2DRawXAxis[k];
        g2D_PositionTotal[1] += g2DRawYAxis[k];
    }

    if(g2D_PositionTotal[0] > 0 || g2D_PositionTotal[1] > 0){
        g2D_Positions[0] = g2D_PositionWeighted[0]/g2D_PositionTotal[0];
        g2D_Positions[1] = g2D_PositionWeighted[1]/g2D_PositionTotal[1];
    }
    //-----------------------------------------