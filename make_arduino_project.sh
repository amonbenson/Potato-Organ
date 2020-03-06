PROJECT_NAME=Potato_Organ

# Cleanup
rm -rf $PROJECT_NAME
mkdir $PROJECT_NAME

# Copy all files
cp src/*.cpp $PROJECT_NAME
cp src/*.h $PROJECT_NAME
mv "${PROJECT_NAME}/main.cpp" "${PROJECT_NAME}/${PROJECT_NAME}.ino"

# Remove the include Arduino line
sed -i "/^\s*#include <Arduino\.h>/d" "${PROJECT_NAME}/${PROJECT_NAME}.ino"

echo "Arduino Project created in directory ${PROJECT_NAME}"
