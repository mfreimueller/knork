# knork

knork is a utility automation tool. Because I am tired of creating deploy-scripts for my various projects, I wrote a single utility that abstracts most of the configuration away.

## Usage

1. Configure knork as discussed in the Configuration-section.
2. In your project folder, run `knork`.

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
                "rsync -ravuP public/ {path}"
        ]
}
```

A template consists of a set of steps that are executed sequentially. You can define variables by using braces: {path}. Your knork.json must then define a variable for "path".