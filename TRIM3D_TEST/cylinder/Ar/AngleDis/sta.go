package main

import (
	"fmt"
	"math"
)

const PI float64 = 3.1415927

func main() {
	staNum := 20
	data := make([][]float64, 5)
	for i := range data{
		data[i] = make([]float64, staNum)
	}

	for true {
		var (
			name string
			x,y,z float64
			Z int
			mass float64
			t int
			energy,cosX,cosY,cosZ float64
		)
		_,err := fmt.Scanf("%s %f %f %f %d %f %d %f %f %f %f", &name, &x, &y, &z, &Z, &mass, &t, &energy, &cosX, &cosY, &cosZ)

		if err!=nil {
			break
		}
		dx := x - 20.0
		dy := y - 0.0
		dz := z - 0.0

		dl := math.Sqrt(dx*dx + dy*dy + dz*dz)
		dx = dx/dl
		angle := math.Acos(dx)
		col := int(angle/(PI/float64(staNum)))
		row := int(t)
		data[row][col]++
	}

	for i := 0; i<staNum; i++ {
		fmt.Println(PI/float64(staNum)*float64(i+1), data[0][i], data[1][i], data[3][i], data[4][i])
	}

}
