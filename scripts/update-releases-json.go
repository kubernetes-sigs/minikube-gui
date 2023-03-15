/*
Copyright 2023 The Kubernetes Authors All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

// This file updates the releases.json file with the OS specific download
// links for the latest version.

package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"log"
	"os"
	"strings"
)

const releasesFile = "../releases.json"

type links struct {
	Darwin  string `json:"darwin,omitempty"`
	Linux   string `json:"linux,omitempty"`
	Windows string `json:"windows,omitempty"`
}

type release struct {
	Links links  `json:"links,omitempty"`
	Name  string `json:"name"`
}

type releases struct {
	Releases []release
}

func (r *releases) UnmarshalJSON(p []byte) error {
	return json.Unmarshal(p, &r.Releases)
}

func main() {
	version := flag.String("version", "", "The version of minikube to create the entry for")
	flag.Parse()

	if *version == "" {
		fmt.Println("The version flag is required and cannot be empty")
		flag.PrintDefaults()
		os.Exit(1)
	}

	if err := updateReleases(*version); err != nil {
		log.Fatal(err)
	}
}

func updateReleases(version string) error {
	r, err := getReleases(releasesFile)
	if err != nil {
		return err
	}

	linkBase := fmt.Sprintf("https://github.com/kubernetes-sigs/minikube-gui/releases/download/%s/minikube-gui", version)
	newRelease := release{
		Links: links{
			Darwin:  fmt.Sprintf("%s-mac.tar.gz", linkBase),
			Linux:   fmt.Sprintf("%s-linux.tar.gz", linkBase),
			Windows: fmt.Sprintf("%s-windows.zip", linkBase),
		},
		Name: strings.TrimPrefix(version, "v"),
	}

	r.Releases = append([]release{newRelease}, r.Releases...)

	if err := updateJSON(r); err != nil {
		return err
	}
	return nil
}

func getReleases(path string) (releases, error) {
	r := releases{}

	b, err := os.ReadFile(path)
	if err != nil {
		return r, fmt.Errorf("failed to read in releases file %q: %v", path, err)
	}

	if err := json.Unmarshal(b, &r); err != nil {
		return r, fmt.Errorf("failed to unmarshal releases file: %v", err)
	}

	return r, nil
}

func updateJSON(r releases) error {
	b, err := json.MarshalIndent(r.Releases, "", "    ")
	if err != nil {
		return fmt.Errorf("failed to marshal releases to JSON: %v", err)
	}

	if err := os.WriteFile(releasesFile, b, 0644); err != nil {
		return fmt.Errorf("failed to write JSON to file: %v", err)
	}
	return nil
}
