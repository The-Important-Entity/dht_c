# dht_c

## Build
```
git clone git@github.com:The-Important-Entity/dht_c.git
cd dht_c
make
./main 2000 100
```

## Usage
```
./main <PORT> <NUM_WORKERS>
```

## TCP message format
Get Binding
```
get foo
```
Insert Binding
```
insert foo bar
```
Delete Binding
```
delete foo
```
## Constraints
Max Key Length: 32

Max Value Length: 32
