# knork

knork is a utility automation tool. Because I am tired of creating deploy-scripts for my various projects, I wrote a single utility that abstracts most of the configuration away.

## Usage

1. Configure knork as discussed in the Configuration-section.
2. In your project folder, run `knork`. This will execute the template found in your `knork.json` file.

Additionally, knork supports running globally defined templates directly from the terminal without the
need to configure a `knork.json` file first. If, for example, you configured the template `hugo` (see below)
and wanted to run it directly, you could run knork as such: `knork --run hugo path=/target/path`.

Running knork in command line mode requires you to specify the globally defined template via `--run <template_name>`
and add all required variables in `key=value` pairs.

## Configuration

knork uses two types of config files. The first must be placed in your project and is called "knork.json":

```
{
        "template": "hugo",
        "variables": {
                "path": "/var/www/"
        }
}
```

* "template" refers to the template to use (more on this in a minute)
* "variables" refers to the variables that your template defines, that you want to replace with project-relative values.

Additionally, your knork.json can also directly define steps to execute, without a template:

```
{
        "steps": [
			"hugo",
			"cp -r public/ /var/www/"
        ]
}
```

The second config type is "templates.json" and resides in "~/.config/knork/". It defines a set of templates that you use in your project-relative knork.json-files:

```
{
        "hugo": [
                "hugo",
                "rsync -ravuP public/ {SERVER}:{path}"
        ],
        "_variables": {
                "SERVER": "127.0.0.1"
        }
}
```

A template consists of a set of steps that are executed sequentially. You can define variables by using braces: {path}. Your knork.json must then define a variable for "path".

## Dependencies

Knork uses [json from nlohmann](https://github.com/nlohmann/json) and [argparse from p-ranav](https://github.com/p-ranav/argparse).