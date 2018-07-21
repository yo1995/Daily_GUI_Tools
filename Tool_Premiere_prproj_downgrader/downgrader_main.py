# -*- coding: utf-8 -*-

import gzip


def main():
    data = gzip.decompress("").decode("utf-8")


if __name__ == '__main__':
    main()
