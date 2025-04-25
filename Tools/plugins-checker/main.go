package main

import (
	"encoding/json"
	"fmt"
	"log"
	"os"
	"slices"
)

type Module struct {
	Name                   string
	Type                   string
	LoadingPhase           string
	AdditionalDependencies []string
}

type Plugin struct {
	Name            string
	Enabled         bool
	TargetAllowList []string
}

type ProjectConfig struct {
	FileVersion       int
	EngineAssociation string
	Category          string
	Discription       string
	Modules           []Module
	Plugins           []Plugin
}

func main() {

	var args []string
	args = os.Args
	file, err := os.Open("../../IGB300_Geme.uproject")
	if err != nil {
		log.Fatal(err)
	}

	data := make([]byte, 1000)
	count, err := file.Read(data)
	if err != nil {
		log.Fatal(err)
	}

	var config ProjectConfig
	err = json.Unmarshal(data[:count], &config)
	if err != nil {
		log.Fatal(err)
	}

	var pluginNames []string
	for _, p := range config.Plugins {
		if !slices.Contains(args, p.Name) {
			fmt.Println(p.Name + " has been added")
		}
		pluginNames = append(pluginNames, p.Name)
	}

	for i, a := range args {
		if !slices.Contains(pluginNames, a) && i != 0 {
			log.Fatal(a + " has been deleted")
		}
	}
}
