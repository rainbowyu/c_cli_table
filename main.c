extern int test_csv_split_on_newline(void);
extern int test_cli_static_table_create(void);
extern int test_csv_parse(void);
extern int test_cli_static_table_csv_create(void);

int main() {
    test_csv_split_on_newline();
    test_csv_parse();
    test_cli_static_table_create();
    test_cli_static_table_csv_create();
    return 0;
}
