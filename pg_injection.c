#include <libinjection.h>
#include <libinjection_sqli.h>
#include <postgres.h>

#include <fmgr.h>
#include <parser/analyze.h>
#include <tcop/tcopprot.h>
#include <utils/guc.h>

PG_MODULE_MAGIC;

static const struct config_enum_entry report_level_options[] = {
    {"debug5", DEBUG5, false},
    {"debug4", DEBUG4, false},
    {"debug3", DEBUG3, false},
    {"debug2", DEBUG2, false},
    {"debug1", DEBUG1, false},
    {"debug", DEBUG2, true},
    {"info", INFO, false},
    {"notice", NOTICE, false},
    {"warning", WARNING, false},
    {"error", ERROR, false},
    {"log", LOG, false},
    {"fatal", FATAL, false},
    {"panic", PANIC, false},
    {NULL, 0, false}
};
static int report_level = WARNING;
static post_parse_analyze_hook_type prev_post_parse_analyze_hook = NULL;

static void my_post_parse_analyze_hook(ParseState *pstate, Query *query, JumbleState *jstate) {
    const char *sql = debug_query_string ? debug_query_string : pstate->p_sourcetext;
    sfilter sf;
    libinjection_sqli_init(&sf, sql, strlen(sql), FLAG_NONE | FLAG_QUOTE_NONE | FLAG_QUOTE_SINGLE | FLAG_QUOTE_DOUBLE | FLAG_SQL_ANSI | FLAG_SQL_MYSQL);
    if (libinjection_is_sqli(&sf)) ereport(report_level, (errcode(ERRCODE_STATEMENT_TOO_COMPLEX), errmsg("SQL injection found"), errdetail("%s", sf.fingerprint)));
    if (prev_post_parse_analyze_hook) prev_post_parse_analyze_hook(pstate, query, jstate);
}

void _PG_init(void); void _PG_init(void) {
    DefineCustomEnumVariable("pg_injection.level", "pg_injection level", NULL, &report_level, WARNING, report_level_options, PGC_USERSET, 0, NULL, NULL, NULL);
    prev_post_parse_analyze_hook = post_parse_analyze_hook;
    post_parse_analyze_hook = my_post_parse_analyze_hook;
}

void _PG_fini(void); void _PG_fini(void) {
    post_parse_analyze_hook = prev_post_parse_analyze_hook;
}
