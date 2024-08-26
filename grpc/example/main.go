package main

import (
	"fmt"

	"github.com/BertoldVdb/go-ais"
	"github.com/BertoldVdb/go-ais/aisnmea"
)

func main() {
	nm := aisnmea.NMEACodecNew(ais.CodecNew(false, false))

	_, _ = nm.ParseSentence("!AIVDM,2,1,0,B,C8tLui00029?mqShpr=M9:v0<bf2,0*57")
	decoded, _ := nm.ParseSentence("!AIVDM,2,2,0,B,L?eQQUeKWeKgS;1001@50Q2P,0*78")
	if decoded != nil {
		fmt.Printf("%+v\n", decoded.Packet)
	}
}
