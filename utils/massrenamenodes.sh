grep -r "base\.any" .
sed -i "s/base\.any/flow.any/g" examples/*.xml plugins-base/tests/*cpp plugins-base/baselibrary.cpp
grep -r "flow\.any" .

