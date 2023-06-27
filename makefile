CC = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
# 使用 gtest 库
GTEST_FLAGS	= -lgtest -lgtest_main -pthread

SRCS = jsonobject.cc jsonparser.cc test_json.cc
OBJS = $(SRCS:.cc=.o)

# gtest测试
G_SRCS = jsonobject.cc jsonparser.cc gtest_json.cc
G_OBJS = $(G_SRCS:.cc=.o)

TARGET = test
G_TARGET = gtest

all: target, gtest

target: $(TARGET)
gtest: $(G_TARGET)

%.o: %.cc
	$(CC) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) $^ -o $@

$(G_TARGET): $(G_OBJS)
	$(CC) $(CXXFLAGS) $^ -o $@ $(GTEST_FLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) $(G_TARGET) $(G_OBJS)

run_test:
	./test

run_gtest:
	./gtest

run:
	./test; ./gtest

H_SRCS = $(SRCS:.cc=.hh)
H_G_SRCS = $(SRCS:.cc=.hh)
FORMAT_STYLE = google

format:
	find . \( -name "*.hh" -o -name "*.cc" \) -exec clang-format -i {} \;
