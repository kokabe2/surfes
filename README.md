# Surfes [![Build Status](https://travis-ci.org/kokabe2/surfes.svg?branch=master)](https://travis-ci.org/kokabe2/surfes)

Surfes is a simple, universal, reusable framework for embedded systems.

## High level architecture

### Package / Component

```plantuml
@startuml component diagram
skinparam {
    defaultFontName Ricty Diminished
    monochrome true
}

package "boot" as boot_package{
    [boot]
    [config]
}

package "core" as core_package{
    [kernel]
    [init]
    [utility]
    [foo]
    [bar]
    [baz]
}

[init] -[hidden]do--> [kernel]
[utility] -[hidden]l--> [kernel]

package "updater" as updater_package{
    [updater]
}

boot_package -[hidden]d-> updater_package
updater_package -[hidden]d-> core_package

@enduml
```

| Package | Purpose                             |
| ------- | ----------------------------------- |
| boot    | Statup the system                   |
| updater | Update components(files)            |
| core    | Provide main features of the system |

### State

```plantuml
@startuml state machine diagram
skinparam {
    defaultFontName Ricty Diminished
    monochrome true
}

[*] --> boot
boot -left-> updater : Runlevel 1
updater --> boot : Return
boot --> core : Runlevel 2 to 5
core --> boot : Return
boot --> boot : Runlevel 6
boot -up-> [*] : Runlevel 0

@enduml
```

#### Runlevel

A runlevel is a mode of operation, and it defines the state of the machine after boot.
Only one runlevel is executed on startup; run levels could be executed one after another.
Default runlevel is 3.

| ID  | Name           | Description                                              |
| --- | -------------- | -------------------------------------------------------- |
| 0   | Halt           | Shuts down the system, power-off if hardware supports it |
| 1   | Recovery mode  | Mode for downloading and recovering files                |
| 2   | Reserved       | Currently unused                                         |
| 3   | User mode      | Starts the system normally                               |
| 4   | Developer mode | Same as runlevel 3 + x                                   |
| 5   | Reserved       | Currently unused                                         |
| 6   | Reboot         | Reboots the system                                       |
