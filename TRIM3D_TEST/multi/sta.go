package main
import (
	"fmt"
)


func main() {
	name,z := "",0.0
	staNum := 20.0
	sumL := 60.0
	dL := sumL/staNum

	mp := make(map[string][]float64)

	for true{
		_,err := fmt.Scanf("%s %f\n", &name, &z)
		if err!= nil {
			break
		}

		_,ok := mp[name]
		if !ok {
			mp[name] = make([]float64, int(staNum))
		}

		if z > 40.0 { z = 39.9 }
		if z < -20.0 { z = -19.9 }

		mp[name][int(-(z-40.0)/dL)]++
	}

	keySlice := make([]string,0)

	for key,_ := range mp {
		keySlice = append(keySlice, key)
	}

	fmt.Println(keySlice)

	for i:=0; i<int(staNum); i++ {
		fmt.Print(float64(i)*dL, " ")
		for j := range keySlice {
			fmt.Print(mp[keySlice[j]][i], " ")
		}
		fmt.Println("")
	}

}
