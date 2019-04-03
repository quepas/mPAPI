% Comment: Hyper-threading might affect the number of available computational threads
function maxCompThreads = findMaxCompThreads(tryMaxThreads)
    % Try to setup maximal number of computational threads
    maxNumCompThreads(tryMaxThreads);
    % First, warm up the JIT and MATLAB
    findCompThreads();
    findCompThreads();
    % Check how many we have found
    foundThreads = findCompThreads();
    % We should find at least two threads (might be not true for uniprocessors)
    maxCompThreads = max(2, numel(foundThreads));
end
