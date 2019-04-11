function pid = pidOf(process_name)
    [~, pid] = system(['pidof ', process_name]);
    pid = str2num(pid);
end
