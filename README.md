# <Scrapper>

## Description

This is a simple scrapping service that allows the user to register the relevant url and the regex pattern to perform scrapping and parsing periodically

## Usage

To Clone: `git clone --recurse-submodules git@github.com:Eli-88/scrapper.git`

To Compile: `python scripts/compile.py`

To Run: `/build/src/scrapper/scrapper --host <local ip addr> --port <local port number>`

To register a url for scrapping

- get/post message to `http://localhost:3456/scrap`

- sample protocol
`{
    "host": "www.reddit.com",
    "port": "443",
    "target": "search?q=Trump%20Georgia&source=trending",
    "version": "1.1",
    "patterns": [
        <regex pattern>
    ]
}`
