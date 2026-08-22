# Helpers shared by the steps of the clean-up-caches action.
#
# Sourced rather than executed, so that every step reports in the same
# shape without repeating it. gh reads GH_TOKEN and GH_REPO from the
# environment.

# GitHub reports what is in use but never the ceiling.
CACHE_LIMIT_MIB=10240

# Summed from the listing rather than read from actions/cache/usage. That
# endpoint lags by minutes, and this runs right after a build, so it would
# report the state from before the run that triggered it.
report_usage() {
	local all used_mib count
	all=$(gh cache list --limit 100 --json sizeInBytes)
	used_mib=$(jq '([.[].sizeInBytes] | add // 0) / 1048576 | floor' <<<"$all")
	count=$(jq 'length' <<<"$all")
	printf '%s: %s MiB of %s MiB across %s caches, %s MiB available\n' \
		"$1" "$used_mib" "$CACHE_LIMIT_MIB" "$count" \
		"$(( CACHE_LIMIT_MIB - used_mib ))"
}

# Every cache when no ref is given.
list_caches() {
	if [ -n "${1:-}" ]; then
		gh cache list --ref "$1" --limit 100 \
			--json id,key,ref,sizeInBytes,createdAt
	else
		gh cache list --limit 100 --json id,key,ref,sizeInBytes,createdAt
	fi
}

tabulate() {
	jq -r 'if length == 0 then "  none"
	       else sort_by(.ref, .key)[]
	            | "  \(((.sizeInBytes/1048576)|floor|tostring)) MiB\t\(.ref)\t\(.key)"
	       end' <<<"$1" | column -t -s "$(printf '\t')"
}

# A cache cannot be overwritten, so each run leaves a new entry behind and
# only the newest of each prefix is ever restored. The key carries the
# timestamp that makes it unique, so grouping strips it.
select_superseded() {
	jq -c '
		group_by(.ref + "|" + (.key | sub("-[0-9]{4}-[0-9]{2}-[0-9]{2}T.*$"; "")))
		| map(sort_by(.createdAt) | .[0:-1])
		| flatten' <<<"$1"
}

total_mib() {
	jq '([.[].sizeInBytes] | add // 0) / 1048576 | floor' <<<"$1"
}
